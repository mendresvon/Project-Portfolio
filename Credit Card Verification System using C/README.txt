The project takes in a card number and determines if it is an invalid card or a valid card.

If it is a valid card:
    it returns a string that says what type of card it is.
    
If it is an invalid card: 
    it return a string saying invalid.

*****!! WARNING: Please red the line below this one !!***** 
The project only works according to the following implementations and specifications:

π ONLY THE MAJOR CARD TYPES ARE INCLUDED (VISA, MASTERCARD, and AMEX);
π The program can only correctly classify card numbers according to the following specifications:
 π―AMEX:
    π American Express uses 15-digit numbers
    π All American Express numbers start with 34 or 37
    
  π―MasterCard
    π MasterCard uses 16-digit numbers
    π Most MasterCard numbers start with 51, 52, 53, 54, or 55 (they also have some other potential starting numbers but only the most common ones are included in this program)
    
  π―Visa 
    πVisa uses 13- and 16-digit numbers.
    πAll Visa numbers start with 4

BUT THAT'S NOT ALL. Please read further to understand the program.

π₯π₯π₯π₯β οΈβ οΈβ οΈβ οΈ MOST IMPORTANT PART OF THE PROGRAM β οΈβ οΈβ οΈβ οΈπ₯π₯π₯π₯
Credit card numbers also have a βchecksumβ built into them, a mathematical relationship between at least one number and others. 
That checksum enables computers and security systems to detect typos (e.g., transpositions), and fraudulent numbers.
So what is this checksum that we are talking about? It's usually the Luhn's Algorithm.


(Source for Information under this line: https://cs50.harvard.edu/x/2022/psets/1/credit/)
ββββββββββββββββββββββ ββββββββββββββββββββββββββββββββββββββ
ββββββββββββββββββββββ ββββββββββββββββββββββββββββββββββββββ
So whatβs the secret formula? Well, most cards use an algorithm invented by Hans Peter Luhn of IBM. According to Luhnβs algorithm, you can determine if a credit card number is (syntactically) valid as follows:

  1. Multiply every other digit by 2, starting with the numberβs second-to-last digit, and then add those productsβ digits together.
  2. Add the sum to the sum of the digits that werenβt multiplied by 2.
  3. If the totalβs last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
 
Thatβs kind of confusing, so letβs try an example with a sample Valid Visa Card: 4003600000000014.

  1. For the sake of discussion, letβs first underline every other digit, starting with the numberβs second-to-last digit:
      4003600000000014

      Okay, letβs multiply each of the underlined digits by 2:
      1β’2 + 0β’2 + 0β’2 + 0β’2 + 0β’2 + 6β’2 + 0β’2 + 4β’2
      
      That gives us:
      2 + 0 + 0 + 0 + 0 + 12 + 0 + 8

      Now letβs add those productsβ digits (i.e., not the products themselves) together:
      2 + 0 + 0 + 0 + 0 + 1 + 2 + 0 + 8 = 13

  2. Now letβs add that sum (13) to the sum of the digits that werenβt multiplied by 2 (starting from the end):
      13 + 4 + 0 + 0 + 0 + 0 + 0 + 3 + 0 = 20

  3. Yup, the last digit in that sum (20) is a 0, so the sample card is legit!

So, validating credit card numbers isnβt hard, but it does get a bit tedious by hand. Which is why we write programs :)).
