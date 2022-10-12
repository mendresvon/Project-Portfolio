import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import *

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ["API_KEY"]:
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Query for the important table information from the database
    current_user = session['user_id']
    table = db.execute(
        'SELECT stock_symbol, stock_name, SUM(shares) as shares, price, SUM(total) as total FROM transactions WHERE user_id = ? GROUP BY stock_symbol', current_user)
    cash = db.execute('SELECT cash FROM users WHERE id = ?', current_user)
    cash = cash[0]['cash']

    total = 0
    for stock in table:
        price = lookup(stock['stock_symbol'])['price']
        sum = price * stock['shares']
        stock.update({'price': usd(price)})
        stock.update({'total': usd(sum)})
        total += sum

    return render_template('index.html', table=table, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == 'POST':
        # Check if user input is valid
        symbol = request.form.get('symbol')
        shares = request.form.get('shares')
        if not symbol:
            flash('Please enter a Stock Symbol')
            return apology('Stock Symbol must not be empty')
        elif not symbol.isalpha():
            flash('Please input valid characters only')
            return apology('Invalid ticker symbol')

        # Check if the stock exists
        stock = lookup(symbol)
        if not stock:
            flash('Stock does not exist')
            return apology('Input a valid Stock Symbol')
        elif not shares or not shares.isnumeric():
            flash('Please input valid numbers')
            return apology('Only numbers are allowed')
        elif int(shares) < 0:
            flash('Please input a positive number')
            return apology('Shares is not a positive integer')

        # Calculates the total transaction cost
        stock_name = stock['name']
        shares = int(shares)
        transaction_cost = shares * stock['price']
        current_user_id = session["user_id"]
        user_cash = db.execute(
            'SELECT cash FROM users WHERE id = :id', id=current_user_id)
        user_cash = user_cash[0]['cash']

        # Deduct the cost of transaction from the user's available cash and update the database
        if user_cash < transaction_cost:
            flash('You can\'t afford that you broke bitch')
            return apology('Insufficient funds')
        remaining_balance = user_cash - transaction_cost
        db.execute('UPDATE users SET cash = ? WHERE id = ?',
                   remaining_balance, current_user_id)

        # Update the transactions table
        date = datetime.datetime.now()
        total = stock['price'] * shares
        db.execute('INSERT INTO transactions (user_id, stock_name, stock_symbol, shares, price, date, total) VALUES (?, ?, ?, ?, ?, ?, ?)',
                   current_user_id, stock_name, symbol.upper(), shares, stock['price'], date, total)

        # Redirect to Portfolio Page
        flash(
            f'Successfully bought {shares} shares of {stock_name} ({symbol}) stock')
        return redirect('/')

    return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']
    table = db.execute('SELECT stock_symbol, shares, price, date FROM transactions WHERE user_id = ?', user_id)
    return render_template("history.html", table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'POST':
        symbol = request.form.get('symbol')
        message = lookup(symbol)
        if message:
            price = message['price']
            price = usd(price)
            return render_template("quoted.html", message=message, price=price)
        elif not symbol:
            flash('Please enter a Stock Symbol')
            return apology('Stock Symbol must not be empty')
        else:
            flash('That stock does not exist in the Stock Market')
            return apology("Invalid Symbol")
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username:
            flash('Please input a username')
            return apology('Username must not be empty')
        elif not password:
            flash('Please input a password')
            return apology('Password must not be empty')
        elif not confirmation:
            flash('Please input password confirmation')
            return apology('Confirmation must not be empty')
        elif password != confirmation:
            flash('Passwords don\'t match!', category='error')
            return apology('Passwords must match!')
        elif not check_password_strength(password):
            flash('Password must contain one uppercase, one lowercase, one digit')
            return apology('Password is too easy to guess')

        try:
            new_user = db.execute('INSERT INTO users (username, hash) VALUES (?, ?)',
                                  username, generate_password_hash(password))
        except:
            flash('Username already exists')
            return apology('Username already exists')

        session['user_id'] = new_user
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session['user_id']

    if request.method == 'POST':
        shares = int(request.form.get('shares'))
        symbol = request.form.get('symbol')
        stock_name = lookup(symbol)['name']
        stock_price = lookup(symbol)['price']
        total_sale = stock_price * shares
        date = datetime.datetime.now()

        # Query for user's owned shares and user's cash
        shares_owned = db.execute(
            'SELECT SUM(shares) as shares FROM transactions WHERE user_id = ? AND stock_symbol like ?', user_id, symbol)
        shares_owned = shares_owned[0]['shares']
        user_cash = db.execute('SELECT cash FROM users WHERE id = ?', user_id)
        user_cash = user_cash[0]['cash']

        # Error Checking
        if not shares:
            flash('You can\'t sell 0 shares')
            return apology('You can\'t sell nothing you stupid bitch')
        elif not symbol:
            flash('Choose a stock to sell')
            return apology('Pick something you asshole')
        elif shares_owned < shares:
            flash('You can\'t sell more than you have')
            return apology('Stop being pretentious you dumb fuck')

        # Sell the stock to the market
        db.execute('INSERT INTO transactions (user_id, stock_name, stock_symbol, shares, price, date, total) VALUES (?, ?, ?, ?, ?, ?, ?)',
                   user_id, stock_name, symbol, (-1)*shares, stock_price, date, total_sale)

        # Declare new user cash
        new_balance = user_cash + total_sale
        db.execute('UPDATE users SET cash = ? WHERE id = ?',
                   new_balance, user_id)

        flash(f'Successfully sold {shares} shares of {stock_name} ({symbol}).')
        return redirect('/')

    stocks = db.execute(
        'SELECT stock_symbol FROM transactions WHERE user_id = ? GROUP BY stock_symbol', user_id)
    return render_template('sell.html', stocks=stocks)
