"""
Project: Vocabulary flashcards (Console App)

Description:
This program helps users improve their English vocabulary by:
1. Reviewing random words and their definitions
2. Testing themselves by guessing the word from its definition (with 5 attempts)

Features:
- Dictionary of words and definitions
- Random word review
- Self-test with feedback and correct answer reveal

Goal:
Practice using dictionaries, random selection, functions, and loops in Python while building a useful language-learning tool.
"""

import random


Words = {
    "Eager": "Showing keen interest or intense desire.",
    "Effort": "The use of physical or mental energy to do something.",
    "Elaborate": "Involving many carefully arranged parts or details.",
    "Embrace": "To hold someone closely in one's arms, especially as a sign of affection.",
    "Endure": "To suffer something painful or difficult patiently.",
    "Enthusiasm": "Intense and eager enjoyment, interest, or approval.",
    "Essential": "Absolutely necessary; extremely important.",
    "Evaluate": "To judge or determine the significance, worth, or quality of something.",
    "Examine": "To inspect or scrutinize carefully.",
    "Expand": "To increase in size, number, or importance.",
    "Fascinate": "To attract and hold the attention of someone.",
    "Fortunate": "Having good luck or receiving good things.",
    "Generate": "To produce or create something.",
    "Gratitude": "The quality of being thankful; readiness to show appreciation.",
    "Hesitate": "To pause before saying or doing something, often due to uncertainty.",
    "Illuminate": "To light up or make something clear.",
    "Imitate": "To copy or mimic someone or something.",
    "Innovate": "To introduce new ideas or methods.",
    "Inspire": "To fill someone with the urge or ability to do something creative.",
    "Investigate": "To carry out a systematic or formal inquiry to discover and examine the facts.",
    "Jubilant": "Feeling or expressing great happiness and triumph.",
    "Knowledge": "Facts, information, and skills acquired through experience or education.",
    "Liberate": "To set someone free from a situation.",
    "Magnify": "To make something appear larger than it is.",
    "Navigate": "To plan and direct the course of a journey.",
    "Nurture": "To care for and encourage the growth or development of someone or something.",
    "Observe": "To notice or perceive something and register it as being significant.",
    "Persist": "To continue firmly in an opinion or course of action despite difficulty.",
    "Proclaim": "To announce something publicly or officially.",
    "Quaint": "Attractively unusual or old-fashioned.",
    "Radiant": "Sending out light; shining or glowing brightly.",
    "Reconcile": "To restore friendly relations between.",
    "Sustain": "To support, hold, or bear up from below.",
    "Transform": "To make a thorough or dramatic change in form, appearance, or character.",
    "Uplift": "To raise the level of; improve.",
    "Venture": "A risky or daring journey or undertaking.",
    "Whimsical": "Playfully quaint or fanciful, especially in an appealing and amusing way.",
    "Yearn": "To have an intense feeling of longing for something.",
    "Zealous": "Having or showing zeal; fervent.",
}


# ============================ THA MAIN CODE ============================
def main():
    while True:
        message = """
1. Review random word
2. Test yourself
3. Exit"""
        print(message)
        user_choice = input("Enter your number of choice: ").strip()

        # 1. if the user choosen to review a random word
        if user_choice == "1":
            review_random()

        elif user_choice == "2":
            test_yourself()

        elif user_choice == "3":
            print("Have a great day")
            break
        else:
            print("please enter a valid choice")


# ========= WORD_DEF_PAIRS FUCNTION =========
def word_def_pairs():
    # get the items of the dict (Key: the word / Value: the definition)
    for word, definition in Words.items():
        # convert every key and value to a big list containing tuples, every tuple containing--> word , definition
        tuple_lst = list(Words.items())
    # choosing a tuple from the big list
    random_tuple = random.choice(tuple_lst)
    # devide the tuple into 2 variables
    word, definition = random_tuple
    return word, definition
    # # we can simplify the func to only this on line
    # word , definition = random.choice(list(Words.items()))


# ============================ REVIEW_A_WORD FUNCTION ============================
def review_random():
    # capturing what what the word_def_pairs func returns
    word, definition = word_def_pairs()
    print("_" * 30)
    print(f"Definition: {definition}\nWord: {word}")
    print("_" * 30)


# ============================ TEST_YOURSELF FUNCTION ============================
def test_yourself():
    # count the attempts
    attempts = 0
    # the user just have 5 attempts
    # if the user have one attempt print --> 'attpempt' without 's'
    word, definition = word_def_pairs()
    print(f"\ndefinition--> {definition}")  # priting the question
    while attempts < 5:
        answer = input("answer: ").strip().title()
        if answer == word:
            print("Correct Answer")
            break
        else:
            attempts += 1
            remainning_attempts = 5 - attempts
            if remainning_attempts == 1:
                print(f"Incorrect answer!\nyou have {remainning_attempts} attempt")
            elif 1 < remainning_attempts <= 5:
                print(f"Incorrect answer!\nyou have {remainning_attempts} attempts")
            else:
                print(f"you have no attempts ramaining\nThe answer is: {word}")


if __name__ == "__main__":
    main()
