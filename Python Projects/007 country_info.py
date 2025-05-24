"""
Project: Student Lookup System

Description:
This console-based tool allows users to enter a student's name or ID
and find out which school the student belongs to.

Features:
- Searches across multiple schools
- Supports lookup by both student name and ID
- Displays school name if the student is found

"""

countries = [
    ("Palestine", "Al-Quds", "Asia", "Arabic", 5000000),
    ("Algeria", "Algiers", "Africa", "Arabic", 42000000),
    ("Bahrain", "Manama", "Asia", "Arabic", 1700000),
    ("Comoros", "Moroni", "Africa", "Arabic", 800000),
    ("Djibouti", "Djibouti", "Africa", "Arabic", 900000),
    ("Egypt", "Cairo", "Africa", "Arabic", 100000000),
    ("Iraq", "Baghdad", "Asia", "Arabic", 40000000),
    ("Jordan", "Amman", "Asia", "Arabic", 10000000),
    ("Kuwait", "Kuwait City", "Asia", "Arabic", 4000000),
    ("Lebanon", "Beirut", "Asia", "Arabic", 6000000),
    ("Libya", "Tripoli", "Africa", "Arabic", 7000000),
    ("Morocco", "Rabat", "Africa", "Arabic", 35000000),
    ("Oman", "Muscat", "Asia", "Arabic", 5000000),
    ("Qatar", "Doha", "Asia", "Arabic", 2700000),
    ("Saudi Arabia", "Riyadh", "Asia", "Arabic", 34000000),
    ("Sudan", "Khartoum", "Africa", "Arabic", 43000000),
    ("Syria", "Damascus", "Asia", "Arabic", 17000000),
    ("Tunisia", "Tunis", "Africa", "Arabic", 11000000),
    ("United Arab Emirates", "Abu Dhabi", "Asia", "Arabic", 10000000),
    ("Yemen", "Sana'a", "Asia", "Arabic", 29000000),
    ("Indonesia", "Jakarta", "Asia", "Indonesian", 2700000000),
    ("Pakistan", "Islamabad", "Asia", "Urdu", 2200000000),
    ("Bangladesh", "Dhaka", "Asia", "Bengali", 1600000000),
    ("Turkey", "Ankara", "Europe/Asia", "Turkish", 80000000),
    ("Iran", "Tehran", "Asia", "Persian", 83000000),
    ("Malaysia", "Kuala Lumpur", "Asia", "Malaysian", 32000000),
    ("Nigeria", "Abuja", "Africa", "English", 200000000),
    ("Ethiopia", "Addis Ababa", "Africa", "Amharic", 110000000),
    ("Albania", "Tirana", "Europe", "Albanian", 2877797),
    ("Andorra", "Andorra la Vella", "Europe", "Catalan", 77006),
    ("Austria", "Vienna", "Europe", "German", 8955102),
    ("Belarus", "Minsk", "Europe", "Belarusian", 9449323),
    ("Belgium", "Brussels", "Europe", "Dutch, French, German", 11519193),
    (
        "Bosnia and Herzegovina",
        "Sarajevo",
        "Europe",
        "Bosnian, Serbian, Croatian",
        3280819,
    ),
    ("Bulgaria", "Sofia", "Europe", "Bulgarian", 6948445),
    ("Croatia", "Zagreb", "Europe", "Croatian", 4105267),
    ("Cyprus", "Nicosia", "Europe", "Greek, Turkish", 1189265),
    ("Czech Republic", "Prague", "Europe", "Czech", 10618303),
    ("Denmark", "Copenhagen", "Europe", "Danish", 5792202),
    ("Estonia", "Tallinn", "Europe", "Estonian", 1324333),
    ("Finland", "Helsinki", "Europe", "Finnish, Swedish", 5540720),
    ("France", "Paris", "Europe", "French", 67102172),
    ("Germany", "Berlin", "Europe", "German", 82979224),
    ("Greece", "Athens", "Europe", "Greek", 10423054),
    ("Hungary", "Budapest", "Europe", "Hungarian", 9769526),
    ("Iceland", "Reykjavik", "Europe", "Icelandic", 341243),
    ("Ireland", "Dublin", "Europe", "English, Irish", 4761657),
    ("Italy", "Rome", "Europe", "Italian", 60461826),
    ("Kosovo", "Pristina", "Europe", "Albanian, Serbian", 1800000),
    ("Latvia", "Riga", "Europe", "Latvian", 1886198),
    ("Liechtenstein", "Vaduz", "Europe", "German", 38375),
    ("Lithuania", "Vilnius", "Europe", "Lithuanian", 2722289),
    ("Luxembourg", "Luxembourg", "Europe", "Luxembourgish, French, German", 625978),
    ("Malta", "Valletta", "Europe", "Maltese, English", 514000),
    ("Moldova", "Chisinau", "Europe", "Romanian", 2883300),
    ("Monaco", "Monaco", "Europe", "French", 39242),
    ("Montenegro", "Podgorica", "Europe", "Montenegrin", 621810),
    ("Netherlands", "Amsterdam", "Europe", "Dutch", 17231017),
    ("North Macedonia", "Skopje", "Europe", "Macedonian", 2083374),
    ("Canada", "Ottawa", "North America", "English, French", 38550000),
    ("United States", "Washington, D.C.", "North America", "English", 329000000),
    ("Mexico", "Mexico City", "North America", "Spanish", 130750000),
    ("Brazil", "Brasília", "South America", "Portuguese", 211200000),
    ("Argentina", "Buenos Aires", "South America", "Spanish", 44480000),
    ("Colombia", "Bogotá", "South America", "Spanish", 50370000),
    ("Peru", "Lima", "South America", "Spanish", 32160000),
    ("Venezuela", "Caracas", "South America", "Spanish", 28200000),
]

# get the inputs
while True:
    inp = input("Enter a country name or capital to search for: ").title().strip()
    check = False  # to fix the else problem
    # get the result
    for i in range(len(countries)):
        if (countries[i][0] == inp) or (countries[i][1] == inp):
            print("\nHere is the result")
            print("-" * 20)
            print(f"Country: {countries[i][0]}")
            print(f"Capital: {countries[i][1]}")
            print(f"Continent: {countries[i][2]}")
            print(f"Language: {countries[i][3]}")
            print(f"Population: {countries[i][4]}")
            print("-" * 20)
            check = True
            break

    if check == False:
        print("\nSorry, we do not have this country in our database !!\n")
