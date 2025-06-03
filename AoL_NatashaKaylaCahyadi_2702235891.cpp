#include<stdio.h> //Included for standard input and output library
#include<stdlib.h> //Included for standard library including memory allocation
#include<string.h> //Included for string manipulation library

typedef struct trienode
{
    //To find the node for that particular char and will return NULL if there's no child for that character (128 is ASCII)
    struct trienode *child[128]; 
    //End of the word (if the node don't have a word, then return 0. if the node have a word, then return 1)
    int terminal; 
    //For a new slang word description
    char desc[1000]; 
};

//Declare root as an empty root
struct trienode *root = NULL;

//Function to create node
trienode *createNode(){
    //Allocate memory for a trienode and puts the memory's address in the pointer variable newNode.
    trienode *newNode = (trienode*)malloc(sizeof(trienode));

    //Initialize that the child of new node is NULL (there is no child nodes)
    for(int i = 0; i < 128; i++){
        newNode->child[i] = NULL;
    }

    //Initialize end of the word is 0 (the node is not end of the word)
    newNode->terminal = 0;
    //Copy an empty string to the description in new node
    strcpy(newNode->desc, "");
    
    return newNode;
}

//Funtion to insert word to trienode
void insertWord(trienode **root, char word[], char desc[]){
    //If root is NULL then add the new character in the node
    if(*root == NULL) *root = createNode(); 
    
    //Create a temporary pointer and initialize with address of the root to traverse the trie while inserting the word
    trienode *temp = *root;
    //Calculate the length of the word that we want to insert
    int len = strlen(word);

    //The loop check if there's a character in the child nodes
    for(int i = 0; i < len; i++){
        //If the child is NULL
        if(temp->child[word[i]] == NULL){ 
            //Create a new node
            temp->child[word[i]] = createNode();
        }
        //Move temp to the child node corresponding to the current character
        temp = temp->child[word[i]];
    }

    //Copy the description of the word into the description of the last node
    strcpy(temp->desc, desc);

    //If there's end of the word, then we update the word
    if(temp->terminal){
        printf("Successfully updated a slang word\n");
        printf("Press enter to continue...\n");
    }
    //If there's no end of the word (there's no child), then we release a new word
    else {
        printf("Successfully released new slang word\n");
        printf("Press enter to continue...\n");
    }

    //Mark that there's already end of the word by setting the terminal into 1
    temp->terminal = 1;
}

//Function for menu 1
void menu1(){
    //Declare input word and description word as character
    char word_input[1000], desc_input[1000];
    //Declare count number as integer
    int countNum;

    //Validate the word must be more than one characters and contains no space
    do
    {
        printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
        scanf("%[^\n]s", word_input); getchar();
    } while (strlen(word_input) <= 1 || strchr(word_input, ' ') != NULL);

    //Validate the description of the word must be more than two words
    do
    {
        printf("Input a new slang word description [Must be more than 2 words]: ");
        scanf("%[^\n]", desc_input); getchar();
        
        //Initialize count number is 0 
        countNum = 0;

        //Iterate each character of the string and count the number of spaces in it
        for(int i = 0; i < strlen(desc_input); i++){
            if(desc_input[i] == ' '){
                countNum++;
            } 
        }
        countNum++;
    } while (countNum < 2);
    //Call function insertWord to add word and description to the trienode (each character of the word will be the child nodes)
    insertWord(&root, word_input, desc_input);
}

//Function to search a word in the trienode
int searchWord(trienode **root, char key[], char desc[]){
    //Create a current node and initialize with address of the root 
    trienode *currNode = *root;
    //Calculate the length of the key that we want to search
    int len = strlen(key);
    
    //Check if the word that we want to search is in the trienode or not by iterates over each character of the word
    for(int i = 0; i < len; i++){
        //If the word is not present in trienode, return 0
        if(currNode->child[key[i]] == NULL){
            return 0;
        }
        //Move current node to the child node corresponding to the current character
        currNode = currNode->child[key[i]];
    }
    //If the word is present in trienode, copy the description in the current node into the description
    strcpy(desc, currNode->desc);
    //Return value of the terminal (If 1, then the word is present. If 0, then the word isn't present)
    return currNode->terminal;
}

//Function for menu 2
void menu2(){
    //Declare key and description as character
    char key[1000];
    char description[1000];
    //Declare result as integer
    int result;

    //Validate the word must be more than one characters and contains no space
    do
    {
        printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
        scanf("%s", key); getchar();
    } while (strlen(key) <= 1 || strchr(key, ' ') != NULL);

    //Call seacrhWord function and store it in the result variable
    result = searchWord(&root, key, description);

    //If result is 0, it means that the word is not present
    if(result == 0){
        printf("There is no word \"%s\" in the dictionary.\n", key);
        printf("Press enter to continue...\n");
    }else{
    //If result is not 0, it means that the word is present and print the word with the description
        printf("Slang word : %s\n", key);
        printf("Description: %s\n", description);
    }
}

//Function to search prefix word in the trienode
int searchPrefix(trienode **root, char prefix[]){
    //Create a current node and initialize with address of the root
    trienode *currNode = *root;
    //Calculate the length of the prefix word that we want to search
    int len = strlen(prefix);

    //Check if the prefix word that we want to search is in the trienode or not by iterates over each character of the word
    for(int i = 0; i < len; i++){
        //If the prefix word is not present in trienode, return 0
        if(currNode->child[prefix[i]] == NULL){
            return 0;
        }
        //Move current node to the child node corresponding to the current character
        currNode = currNode->child[prefix[i]];
    }
    //Return 1 if the prefix word is present in trienode
    return 1;
}

//Function to print all of the word in the trienode by lexicographically
void printAll(trienode *root, char word[], int len, int *count){
    //Declare new word as character with size length of the word + 2
    //Len + 2 will give enough space to accommodate the characters of the word plus an extra character and NULL terminator
    char newWord[len + 2];
    //Copy each character of word to the new word
    memcpy(newWord, word, len);
    //Set the character at index len + 1 of new word to NULL terminator, indicating the end of the string
    newWord[len + 1] = '\0';
    //Create a temporary pointer and initialize with root 
    trienode *temp =  root;
    //If the root is NULL, returns immediately (there are no words to print)
    if(root == NULL) return;

    //Iterates each character of the word and updating temp to child node corresponding to each character
    for(int i = 0; i < strlen(word); i++){
        temp = temp->child[word[i]];
    }

    //If terminal (end of the word) of the temp is not 0, print the word
    if(temp->terminal){
        printf("%d. %s\n", (*count), word);
        (*count)++;
    }

    //Check if child node is not NULL, update new word with current character and recursively call printAll function
    for(int i = 0; i < 128; i++){
        if(temp->child[i] != NULL){
            newWord[len] = i;
            printAll(root, newWord, len + 1, count);
        }
    }
}

//Function for menu 3
void menu3(){
    //Declare prefix and temp as character
    char prefix[1000];
    char temp[1000];
    //Declare result and count as integer
    int result;
    //Count start with 1
    int count = 1;

    //Ask a user to input the prefix to be searched
    printf("Input a prefix to be searched: ");
    scanf("%s", prefix); getchar();

    //Calculate the length of the prefix word
    int len = strlen(prefix);
    //Call searchPrefix function and store it in the result variable
    result = searchPrefix(&root, prefix);
    //Check if there's prefix word or not and print the word if the word is present in the trienode
    if(result){
        printf("Words starts with \"%s\":\n", prefix);
        printAll(root, prefix, len, &count);
    }else{
        printf("There is no prefix \"%s\" in the dictionary\n", prefix);
    }
    printf("Press enter to continue...\n");
}

//Function for menu 4
void menu4(){
    //Declare word as character
    char word[1000] = "";
    //Declare count as integer and start with 1
    int count = 1;
    //Check is there's word in the trienode or not and print all of the words if there's word in trienode
    if(root == NULL){
        printf("There is no slang word yet in the dictionary.\n");
    }else{
        printf("List of all slang words in the dictionary:\n");
        printAll(root, word, strlen(word), &count);
    }
    printf("Press enter to continue...\n");
}

void displayAll(){
    printf("\t\t\tBoogle\n");
    printf("===========================================================\n");
    printf("Menu:\n");
    printf("1. Release a new slang word\n");
    printf("2. Search a slang word\n");
    printf("3. View all slang words starting with a certain prefix word\n");
    printf("4. View all slang words\n");
    printf("5. Exit\n");
    printf("Choice: ");

}

int main(){
    //Declare root as an empty root
    trienode *root = NULL;
    //Declare choice as integer
    int choice;

    //The menu is displayed repeatedly as long as user's choice is not 5 (If user's choice is 5, it'll end of the program)
    do
    {
        //User should input choice between 1 until 5 or the menu will display the menu and ask user to input the choice again
        do
        {   
            //Ask user to input the choice
            displayAll();
            scanf("%d", &choice); getchar();

            //Execute the corresponding menu function based on the user's choice
            switch (choice)
            {
            case 1:
                //Called the function for menu 1 if user's choice is 1
                menu1();
                break;
            case 2:
                ////Called the function for menu 2 if user's choice is 2
                menu2();
                break;
            case 3:
                //Called the function for menu 3 if user's choice is 3
                menu3();
                break;
            case 4:
                //Called the function for menu 4 if user's choice is 4
                menu4();
                break;
            case 5:
                //Print the farewell message if user's choice is 5
                printf("Thank you... Have a nice day :)\n");
                break;
            }
        } while (choice < 1 && choice > 5); 
    } while (choice != 5);
    
    return 0;
}