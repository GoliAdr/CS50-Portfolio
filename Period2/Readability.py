from cs50 import get_string

def count_letters(text):            #funtion to count the letters
    letters = 0                     #counter set to 0
    for char in text:               #loop for every character
        if char.isalpha():          #checks the letter (a-z A-Z)
            letters += 1            #counter +1
    return letters                  #returns the count

def count_words(text):              #counts the words
    words = 1                       #words at 1 because first word isn't followed by a space
    for char in text:               #loop for every character
        if char == " ":             #Space character
            words += 1              #every space +1
    return words                    #word count

def count_sentences(text):
    sentences = 0
    for char in text:
        if char in [".", "!", "?"]: #looks at the symbols
            sentences += 1          #counts
    return sentences                #all counts

def main():
    text = get_string("Text: ")     #Asks for the sentence
    letters = count_letters(text)   #calls helper fuctions and stores heir result
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters * 100 / words       #average letters and sentences per 100 words
    S = sentences * 100 / words
    grade = round(0.0588 * L - 0.296 * S - 15.8)    # formula

    if grade >= 16:
        print("Grade 16+")          #Grade 16+
    elif grade < 1:
        print("Before Grade 1")     #grade before 1
    else:
        print(f"Grade {grade}")     #shows the exact grade

main()
