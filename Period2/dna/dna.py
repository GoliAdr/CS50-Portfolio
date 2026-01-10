import csv
import sys


def main():

                                                        # Check for command-line usage (need 2 arguments: database + sequence)
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    with open(sys.argv[1]) as database_file:            # opens the database file
        reader = csv.DictReader(database_file)          # reads CSV as list of dicts
        database = list(reader)                         # converts to list for iteration

    # Read DNA sequence file into a variable
    with open(sys.argv[2]) as sequence_file:            # opens the DNA sequence file
        sequence = sequence_file.read()                 # reads the entire DNA string

    # Find longest match of each STR (Short Tandem Repeat) in the DNA sequence
    str_names = list(database[0].keys())[1:]            # takes all STR names except "name"
    str_counts = {}                                     # empty dictionary to store counts

    for STR in str_names:                               # loop over each STR type
        str_counts[STR] = longest_match(sequence, STR)  # find longest repeat and store it

    # Check database for matching profiles
    for person in database:                             # loop over every person in database
        match = True                                    # assume it's a match until proven false

        for STR in str_names:                           # check each STR for that person
            if int(person[STR]) != str_counts[STR]:     # compare database value to found value
                match = False                           # if one STR doesn’t match it is not same person
                break                                   # stop checking further STRs

        if match:                                       # if all STRs match, we found the person
            print(person["name"])                       # print their name
            return                                      # end

    # If no match found after checking everyone
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


main()
