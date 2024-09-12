#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#define MAX_NUMBER_OF_ACCOUNTS 200
#define MAX_LENGTH_USERNAME 50
#define MAX_LENGTH_PASSWORD 50
#define MAX_LENGTH_NAME 200
#define MAX_LENGTH_EMAIL 200
#define MAX_LENGTH_PHONE 12
#define MAX_LENGTH_BUFFER 1024
#define MAX_LENGTH_INPUT 20
#define MAX_LENGTH_ACCOUNT 15
#define MAX_FILE_NAME 15
#define MAX_LENGTH_LINE 1000
int numberOfaccounts;
unsigned int transaction_id;
typedef struct
{
    char username[MAX_LENGTH_USERNAME];
    char password[MAX_LENGTH_PASSWORD];
} User;

typedef struct
{
    int month;
    int year;
} Date;

typedef struct
{
    char account_number[MAX_LENGTH_ACCOUNT] ;
    char name[MAX_LENGTH_NAME];
    char email_address[MAX_LENGTH_EMAIL];
    double balance;
    char mobile[MAX_LENGTH_PHONE];
    Date date;
} Account;

//FUNCTION PROTOTYPES
void Load (Account account[],int max_Num);
User LOGIN ();
int isValidCredentials(User user);
int validate_name(char name[]);
int validate_phone_number(char phone_number[]);
int validate_account_number(char account_number[]);
int validate_email(char email[]);
int validate_month(char month[]);
int validate_year(char year[]);
void save(Account accounts[]);
void loginMenu(Account accounts[]);
void menu(Account accounts[]);
int check_account_number(Account account[],char entered_account_num[]);
void sortByName(Account accounts[]);
char* monthInletters(int month);
void sortByDate(Account accounts[]);
void sortByBalance(Account accounts[]);
void PRINT(Account accounts[]);
void generateRandomAccountNumber(Account *newAccount, Account account[]);
void TimeStamp(int* year, int* month , int* day, int* hour, int* min, int* sec);
void Add(Account account[]);
void Delete(Account account[]);
void Modify(Account account[]);
void transfer(Account account[]);
void deposit(Account account[]);
void withdraw(Account account[]);
void navigate();
void report();
void quit();
void query(Account accounts[]);
void displayAccount(Account account);
void advancedSearch(Account account[]);
int isDouble(const char *str, double* balance);

//load function
void Load (Account account[],int max_Num)
{
    numberOfaccounts=0;
    int i;
    FILE *f2;
    const char DELIMETER[] = ",-\n";
    char *token;
    char buffer[MAX_LENGTH_LINE];
    char filename[MAX_FILE_NAME];

    FILE *f = fopen("accounts.txt","r");
    //Check if file exists
    if (f==NULL)
    {
        printf("File doesn't exist");
        fclose(f);
        exit(-1);
    }
        //Load Account data from file
        for(i=0; !feof(f) && i<max_Num; i++) //Ensures not reading beyond the specified accounts
        {
            numberOfaccounts++;

            if (!fgets(buffer,sizeof(buffer),f))
            {
                //Validate entered data for user
                printf("\n  Error in stored data\n");
                exit(-1);
            }
            token = strtok(buffer,DELIMETER);
            strcpy(account[i].account_number,token);
            if(!validate_account_number(account[i].account_number))
            {
                printf("\n  Error in account number stored on the system.\n");
                exit(-1);
            }

            token = strtok(NULL,DELIMETER);
            strcpy(account[i].name,token);
            if(!validate_name(account[i].name))
            {
                printf("\n  Error in name stored on the system.\n");
                exit(-1);
            }


            token = strtok(NULL,DELIMETER);
            strcpy(account[i].email_address,token);
            if(!validate_email(account[i].email_address))
            {
                printf("\n  Error in email address stored on the system.\n");
                exit(-1);
            }


            token = strtok(NULL,DELIMETER);
            if(!isDouble(token,&account[i].balance))
            {
            printf("\n  Error in balance stored on the system.\n");
            exit(-1);
            }



            token = strtok(NULL,DELIMETER);
            strcpy(account[i].mobile,token);
            if(!validate_phone_number(account[i].mobile))
            {
                printf("\n  Error in mobile number stored on the system.\n");
                exit(-1);
            }


            token = strtok(NULL,DELIMETER);
            if(!validate_month(token))
            {
                printf("\n  Error in month stored on the system.\n");
                exit(-1);
            }

            account[i].date.month = atoi(token); //Convert string to int

            token = strtok(NULL,DELIMETER);
            if(!validate_year(token))
            {
                printf("\n  Error in year stored on the system.\n");
                exit(-1);
            }
            account[i].date.year = atoi(token); //Convert string to int

       snprintf(filename,sizeof(filename),"%s.txt",account[i].account_number);
       f2=fopen(filename,"a"); //write a new .txt file for account with the account number
        if (f2 == NULL) {
        printf("\nError opening file.\n");
        exit(-1);
            }
        fclose(f2);
            }
        f2= fopen("Counter.txt","r");
    if (fscanf(f2, "%d", &transaction_id) != 1) {
        // fscanf returns the number of successfully read items,
        // so if it's not 1, there was an issue reading the integer.
        printf("Error reading from file\n");
        exit(-1);
    }
        if (f2 == NULL) {
        printf("Error opening file\n");
        exit(-1); }
        fclose(f2);
        fclose(f);

}

//login function
User LOGIN ()
{
    User client;
    printf("\nEnter username : ");
    scanf("%s",client.username);
    char ch;
    int i=0;
    // Scanning the password without displaying it in the terminal
    printf("\nEnter password : ");
    while (i < MAX_LENGTH_PASSWORD - 1)
    {
        ch = getch(); // Get a character without echoing it to the terminal (included in library conio.h)
        if (ch == '\r' || ch == '\n')
        {
            break; // Break if Enter key is pressed
        }
        else if (ch == '\b' && i > 0) {
            // Handle backspace: move back and erase the last character
            printf("\b \b");
            i--;
        } else if (ch >= 32 && ch <= 126) {
            // Print '*' for printable characters (excluding control characters)
            client.password[i++] = ch;
            printf("*");
        }
    }
    client.password[i] = '\0';
    return client;
}
// NAVIGATE FUNCTION
void navigate()
{
    char input[MAX_LENGTH_INPUT];
    printf("\n1. Go back to Menu\n");
    printf("2. Quit\n");
    do //do while loop was to make sure that the user entered 1 or 2
    {
        printf("\nChoose an option to continue: ");
        scanf("%s",input);
    }
    while(strcmp(input,"1")!=0&&strcmp(input,"2")!=0);
    if(strcmp(input,"2")==0)
    {
        quit();
    }
}

//valid credentials function
int isValidCredentials(User user)
{
    FILE *userfile;
    userfile = fopen("users.txt", "r"); // Open the file in read mode
    if (userfile == NULL)   // Check if the file was opened successfully
    {
        printf("Error opening file");
        return 1; // Return an error code
    }
    int found=0; // A flag to show if the credentials exist in the file
    User userFromFile; // stores the scanned credentials from the file.
    while (fscanf(userfile, "%s %s", userFromFile.username, userFromFile.password)==2)    //The return value of fscanf is the number of items successfully read.
    {
        if (strcmp(userFromFile.username, user.username) == 0 && strcmp(userFromFile.password, user.password) == 0)
        {
            fclose(userfile);
            found = 1;
        }
    }
    fclose(userfile);
    return found;
}

//validate name

int validate_name(char name[])
{
    int length = strlen(name);
    int flag=1,i;
    for(i=0;i<length;i++)
    {
        if(!((name[i]>='a'&&name[i]<='z')||(name[i]>='A'&&name[i]<='Z')||(name[i]==' ')||(name[i]=='.')))
        {
            flag=0;
            break;
        }
    }
    if(flag==1&&length<MAX_LENGTH_USERNAME)
        return 1;
    return 0;
}



//validate phone number

int validate_phone_number(char phone_number[])
{
    int i,flag=1;
    int length=strlen(phone_number);
    for(i=0; i<length; i++)
    {
        if(!(isdigit(phone_number[i])))
        {
            flag=0;
            break;
        }
    }
    if(flag==1&&length<=15)//max number internationally contains 15 digits
        return 1;
    return 0;
}

//validate account number


int validate_account_number(char account_number[])
{
    int i,flag=1;
    int length=strlen(account_number);
    for(i=0; i<length; i++)
    {
        if(!(isdigit(account_number[i])))
        {
            flag=0;
            break;
        }
    }
    if(length==10 && flag==1)
        return 1;
    return 0;
}

//validate email

int validate_email(char email[])
{
    int i,count_At=0,count_period=0,flag=1;
    int length = strlen(email);
    for(i=0;i<length;i++)
    {
        if(email[i]==' ')
        {
            flag=0;
            break;
        }
        if(email[i]=='@')
            count_At++;
        else if(email[i]=='.')
            count_period++;
    }
    if(count_period>0&&count_At==1&&flag==1)
        return 1;
    return 0;
}
//validate month stored
int validate_month(char month[])
{
    int i;
    for(i=0;i<strlen(month);i++)
    {
        if(!isdigit((unsigned char)month[i]))
            return 0;
    }
    int _month = atoi(month);
    if(_month<1 || _month>12)
        return 0;
    return 1;
}

//validate year stored
int validate_year(char year[])
{
    int i;

    for(i=0;i<strlen(year);i++)
    {
        if(!isdigit((unsigned char)year[i]))
            return 0;
    }
    int _year = atoi(year);
    if(_year < 0)
    return 0;
    return 1;

}

void displayAccount(Account account) {
    char *month=monthInletters(account.date.month);
    printf("\nAccount Number: %s\n", account.account_number);
    printf("Name: %s\n", account.name);
    printf("E-mail: %s\n", account.email_address);
    printf("Balance: %.3lf $\n", account.balance);
    printf("Mobile: %s\n", account.mobile);
    printf("Opened: %s %d\n", month , account.date.year);
}

char *toUpperStr(char str[]){
    int i;
    char *s=malloc(strlen(str)+1);
    for(i=0; i<strlen(str); i++){
        s[i]=toupper(str[i]);
    }
    s[i]='\0';
    return s;
}

void advancedSearch(Account account[])
{
    char keyword[50];
    int found = 0;
    Account userAccount;
    int i;
    size_t len;

    printf("Enter keyword: ");
    fflush(stdin);
    fgets(keyword,sizeof(keyword),stdin);
    len = strlen(keyword);
    keyword[len-1] = '\0'; //remove newline character at the end of string
    printf("Search results:\n\n");

    for (i = 0; i < numberOfaccounts; i++)
    {
        if (strstr(toUpperStr(account[i].name),toUpperStr(keyword))!=NULL){
            found = 1;
            // print account details
            userAccount = account[i];
            displayAccount(userAccount);
            printf("\n");}
    }
    if (!found)
    {
        printf("Not found!\n");
    }
navigate();
}


void query(Account account[])
{
    char enteredAccountNum[50];
    // Get account number input from the user
    printf("Enter Account Number: ");
    scanf("%s", enteredAccountNum);

    int found = 0;
    if (validate_account_number(enteredAccountNum))////check if entered account number is numeric and has 10 numbers
    {
        for (int i = 0; i < numberOfaccounts; i++)
        {
            if (!strcmp(enteredAccountNum,account[i].account_number))
            {
                found = 1;
                // print account details
                displayAccount(account[i]);
                break;
            }
        }
        if (!found)
        {
            printf("Account not found for Account Number: %s\n", enteredAccountNum);
        }
    }
    else
        printf("Invalid account number\n");//entered accounted isn't numeric or doesn’t have 10 numbers
   navigate();
}

//save function

void save(Account accounts[])
{
    FILE *f;
    f= fopen("accounts.txt","w");
    if(f==NULL)
    {
        printf("Error opening the file");
        exit(1);
    }
    int i;

        for(i=0; i<numberOfaccounts; i++)
        {
     if(i)  //to print a new line after each user except the first one
 fprintf(f,"\n");
  fprintf(f,"%s,%s,%s,%.3lf,%s,%d-%d",accounts[i].account_number,accounts[i].name,accounts[i].email_address,accounts[i].balance,accounts[i].mobile,accounts[i].date.month,accounts[i].date.year);
        }


    fclose(f);

}
//quit
void quit()
{
         FILE *f=fopen("Counter.txt","w");
     if(f==NULL)
     {
         printf("Error opening file\n");
         exit(-1);
     }
     fprintf(f,"%d",transaction_id);
     exit(0);
}
void loginMenu(Account accounts[]) {
    User client;
    while(1) {
        char input[MAX_LENGTH_INPUT];
        printf("\tMENU:\n\n");
        printf("1. LOGIN\n");
        printf("2. QUIT\n");

        printf("\nChoose an option to continue: ");
        scanf("%s",input);
        system("cls");
        if(strcmp(input,"1")==0) {
            client=LOGIN();
            if(isValidCredentials(client))
                menu(accounts);
            else {
                printf("\n\nINVALID USERNAME OR PASSWORD.\n\n");
            }
        } else if(strcmp(input,"2")==0) {
            quit();
        }
	else printf("INVALID INPUT\n\n");
    }
}

//menu function
void menu(Account accounts[]) {

    while(1) {
        system("cls");
        printf("\tMENU:\n\n");
        printf(" 1. ADD\n");
        printf(" 2. DELETE\n");
        printf(" 3. MODIFY\n");
        printf(" 4. SEARCH BY ACCOUNT NUMBER (Query)\n");
        printf(" 5. SEARCH BY ACCOUNT NAME (Advanced Search)\n");
        printf(" 6. WITHDRAW\n");
        printf(" 7. DEPOSIT\n");
        printf(" 8. TRANSFER\n");
        printf(" 9. REPORT\n");
        printf("10. PRINT\n");
        printf("11. LOG OUT\n");
        printf("12. QUIT\n");

        printf("\nChoose an option to continue: ");
        char input[MAX_LENGTH_INPUT];
        scanf("%s",input);

        if (strcmp(input,"1")==0){
            system("cls");
            Add(accounts);
        } else if (strcmp(input,"2")==0) {
            system("cls");
            Delete(accounts);
        } else if (strcmp(input,"3")==0) {
            system("cls");
            Modify(accounts);
        } else if (strcmp(input,"4")==0) {

            system("cls");
      query(accounts);
        } else if (strcmp(input,"5")==0) {
            system("cls");
	advancedSearch(accounts);
        } else if(strcmp(input,"6")==0) {
            system("cls");
            withdraw(accounts);
        } else if(strcmp(input,"7")==0) {
            system("cls");
            deposit(accounts);
        } else if(strcmp(input,"8")==0) {
            system("cls");
            transfer(accounts);
        } else if(strcmp(input,"9")==0) {
            system("cls");
             report();
        } else if(strcmp(input,"10")==0) {
            system("cls");
            PRINT(accounts);
        } else if (strcmp(input,"11")==0) {
            system("cls");
	break;
        } else if (strcmp(input,"12")==0) {
            quit();
        }

    }
}




//function to check if account number exists in the accounts array
int check_account_number(Account account[],char entered_account_num[])
{
    int i,flag=0;
    for (i=0; i<numberOfaccounts; i++)
    {
        if(!strcmp(entered_account_num,account[i].account_number))
//compare entered account number with the account numbers in array
        {
            flag = 1;
            break;
        }

    }
    return flag; //returns 1 if account exists and zero if it doesn't
}


//sort function
void sortByName(Account accounts[])
{
    //Bubble sort by name
    int i,pass;
    int sorted=0;
    Account temp;
    for(pass=1; pass<numberOfaccounts && !sorted; pass++)
    {
        sorted=1;
        for(i=0; i<numberOfaccounts-pass; i++)
        {
            if(stricmp(accounts[i].name,accounts[i+1].name)>0)
            {
                temp=accounts[i];
                accounts[i]=accounts[i+1];
                accounts[i+1]=temp;
                sorted=0;
            }
        }
    }
}
char* monthInletters(int month){ //Convert month from numbers to letters
    char *m=malloc(10);
    switch(month){
        case 1 : m="January";
                    break;
        case 2 : m="February";
                    break;
        case 3 : m="March";
                    break;
        case 4 : m="April";
                    break;
        case 5 : m="May";
                    break;
        case 6 : m="June";
                    break;
        case 7 : m="July";
                    break;
        case 8 : m="August";
                    break;
        case 9 : m="September";
                    break;
        case 10 : m="October";
                    break;
        case 11 : m="November";
                    break;
        case 12 : m="December";
                    break;
    }
    return m;
}
void sortByDate(Account accounts[])  //Bubble sort by date
{
    int i,pass;
    int sorted=0;
    Account temp;
    for(pass=1; pass<numberOfaccounts && !sorted; pass++)
    {
        sorted=1;
        for(i=0; i<numberOfaccounts-pass; i++)
        {
            if((accounts[i].date.year>accounts[i+1].date.year)||(accounts[i].date.year == accounts[i+1].date.year && accounts[i].date.month > accounts[i+1].date.month))
            {
                temp=accounts[i];
                accounts[i]=accounts[i+1];
                accounts[i+1]=temp;
                sorted=0;
            }
        }
    }
}

void sortByBalance(Account accounts[])  //Bubble sort by balance
{
    int i,pass;
    int sorted=0;
    Account temp;
    for(pass=1; pass<numberOfaccounts && !sorted; pass++)
    {
        sorted=1;
        for(i=0; i<numberOfaccounts-pass; i++)
        {
            if(accounts[i].balance>accounts[i+1].balance)
            {
                temp=accounts[i];
                accounts[i]=accounts[i+1];
                accounts[i+1]=temp;
                sorted=0;
            }
        }
    }
}

void PRINT(Account accounts[])
{
    int i;
    char input[MAX_LENGTH_INPUT];
    do
    {
        printf("1. Sorted by name\n2. Sorted by date\n3. Sorted by balance\n");
        printf("Choose an option to display accounts data : ");
        scanf("%s",input);
        if (strcmp(input,"1")==0)
        {
            sortByName(accounts);
            for(i=0; i<numberOfaccounts; i++)
                displayAccount(accounts[i]);

            navigate();
        }
        else if (strcmp(input,"2")==0)
        {
            sortByDate(accounts);
            for(i=0; i<numberOfaccounts; i++)
                displayAccount(accounts[i]);

            navigate();
        }
       else if (strcmp(input,"3")==0)
        {
            sortByBalance(accounts);
            for(i=0; i<numberOfaccounts; i++)
                displayAccount(accounts[i]);

            navigate();
        }
        else printf("\nINVALID INPUT.\n");
    }
    while(strcmp(input,"1")!=0 && strcmp(input,"2")!=0 && strcmp(input,"3")!=0);
}

  //Get current Time
  void TimeStamp(int* year,int* month ,int* day,int* hour, int* min, int* sec)
  {
    time_t t = time(NULL); //returns number of seconds since the Epoch (January 1, 1970,      00:00:00 UTC).
    struct tm current_date = *localtime(&t);
   //current_date.tm_year returns number of years since 1900
    *year = current_date.tm_year + 1900; //add 1900 to get current year
    *month = current_date.tm_mon + 1; //add 1 as index of months is from 0-11
    *day = current_date.tm_mday;
    *hour = current_date.tm_hour;
    *min = current_date.tm_min;
    *sec = current_date.tm_sec; }

  //Generate random but unique 10-digit account number for user
   void generateRandomAccountNumber(Account *newAccount, Account account[]) {
    int flag;

    do {

    // Generate a 10-digit account number then stores the formatted string in the account_number array of characters, while specifying size of account_number to prevent buffer overflow

snprintf((*newAccount).account_number,sizeof((*newAccount).account_number),"970%07d", rand()); //rand() is a function in standard library that generates a pseudo-random number

 flag = 1; //Set the flag to 1 initially to assume generated account number is unique


 for (int i = 0; i < numberOfaccounts; i++) {

         if (strcmp((*newAccount).account_number, account[i].account_number) == 0) {

          flag = 0; // If the generated account number is not unique, set flag to 0 and break
               break;
            }
        }
    } 	while (!flag);   // Continue generating until a unique account number is generated


}
//ADD function
void Add(Account account[]) {

    char filename[MAX_FILE_NAME];
    Account newAccount; //new variable of type Account to store entered fields in
    size_t len; //size_t is a data type that represents the size of an object, it's an unsigned integer type
    char input[20];

   //scans data for the account from user

    generateRandomAccountNumber(&newAccount,account);
    printf("Enter name: ");
    fflush(stdin); //flush any remaining newline characters
    fgets(newAccount.name,sizeof(newAccount.name), stdin); //scans name from user preventing overflow
    len = strlen(newAccount.name);
    newAccount.name[len-1] = '\0'; //remove newline character at the end of string
    //validate name
    if(!validate_name(newAccount.name))
    {
        printf("\nInvalid name.\n");
        navigate();
        return;
    }
    //validate email address
    printf("Enter email address: ");
    scanf("%s",newAccount.email_address);
    if(!validate_email(newAccount.email_address))
    {
        printf("\nInvalid email address.\n");
        navigate();
        return;
    }

    //validate mobile number
    printf("Enter mobile: ");
    scanf("%s",newAccount.mobile);
    if(!validate_phone_number(newAccount.mobile))
    {
        printf("\nInvalid mobile number.\n");
        navigate();
        return;
    }

    newAccount.balance = 0.0; //set balance to zero

    //Get the current time

    time_t t = time(NULL); //returns number of seconds since the Epoch (January 1, 1970, 00:00:00 UTC).
    struct tm current_date = *localtime(&t);
   //current_date.tm_year returns number of years since 1900
    newAccount.date.year = current_date.tm_year + 1900; //add 1900 to get current year
    newAccount.date.month = current_date.tm_mon + 1; //add 1 as index of months is from 0-11

    printf("Do you want to save?\n");
    printf("1. YES\n");
    printf("2. NO\n");
    do
    {
        printf("Enter choice 1 or 2 to continue: ");
        scanf("%s",input);
    }
    while(strcmp(input,"1")!=0&&strcmp(input,"2")!=0); //to ensure user enters 1 or 2
   //save changes
    if (strcmp(input,"1")==0)
    {
     numberOfaccounts++;
    account[numberOfaccounts-1] = newAccount; //copy the changes to original array
    save(account); //call save function to store changes in accounts.txt file
    printf("\nAccount added succesfully.\n");
    strcpy(filename,account[numberOfaccounts-1].account_number);
    strcat(filename,".txt"); //filename with the account number

    FILE *f2 = fopen(filename,"w"); //open a new file for account with the account number
    if (f2==NULL)
    {printf("\nError opening file.\n"); //to handle error for opening file
    fclose(f2);
    exit(-1); }

    fclose(f2); //close file
    }
    else
//discard changes
   { printf("\nChanges are discarded\n"); }

navigate();

}

//DELETE account function
void Delete(Account account[])
{
    int i;
    char deleted_accountnum[MAX_LENGTH_ACCOUNT];
    char filename[MAX_FILE_NAME];
    int flag;
    char input[MAX_LENGTH_INPUT];
    int j;


    printf("Enter account number you want to delete: ");
    scanf("%s",deleted_accountnum); //get account number user wants to delete

    if(!validate_account_number(deleted_accountnum))
    {
        printf("\nInvalid account number.\n");
        navigate();
        return;
    }

    //Check if account exists
    flag = check_account_number(account,deleted_accountnum);
    if (!flag)
       { printf("\nAccount number isn't found.\n");
	navigate();
    return; }

    for(i=0;i<numberOfaccounts;i++)
    if(!strcmp(account[i].account_number,deleted_accountnum))
    break;

//Reject deletion of account if balance doesn’t equal zero
    if (account[i].balance != 0.0)
      {printf("\nDeletion rejected, account balance is greater than zero.\n");
   navigate();
    return; }

//Confirm if user wants to save the changes
    printf("Do you want to save?\n");
    printf("1. YES\n");
    printf("2. NO\n");
    do
    {
        printf("Enter choice 1 or 2 to continue:  ");
        scanf("%s",input);
    }

    while(strcmp(input,"1")!=0&&strcmp(input,"2")!=0); //ensure that input is either 1 or 2


    if (strcmp(input,"1")==0)
    {
    strcpy(filename,account[i].account_number);
    strcat(filename,".txt");
    if(remove(filename))  //Handle error in case the file doesn't get deleted
       {printf("\nError deleting file\n");
       exit(-1); }

    numberOfaccounts--; //decrement number of accounts as one account is deleted

    //delete account from array
    for(j=i;j<numberOfaccounts;j++)
    account[j]=account[j+1];
   //save changes to accounts.txt file
    save(account);
    printf("\nAccount deleted succesfully.\n");

    }
    else
      //Discard changes
    {printf("\nChanges are discarded.\n");}

navigate();
}
//MODIFY account details function
void Modify(Account account[])
{
    int i;
    Account modified_account; //variable of type Account to store modifications in
    char modified_accountnum[MAX_LENGTH_ACCOUNT];
    char input[20];

   //scans account number for the account to be modified
    printf("Enter account number: ");
    scanf("%s",modified_accountnum);

    if(!validate_account_number(modified_accountnum))
    {
        printf("\nInvalid account number.\n");
        navigate();
        return;
    }

   //check if account exists
    for (i=0;i<numberOfaccounts;i++)
    {
        if(!strcmp(modified_accountnum,account[i].account_number))
        break;
    }
    if (!check_account_number(account,modified_accountnum))
       { printf("\nAccount number isn't found.\n");
    navigate();
    return; }

   //get new changes in the account fields from user
    printf("New name: ");
    fflush(stdin); //flush any remaining newline characters
    fgets(modified_account.name,sizeof(modified_account.name), stdin); //scans name from user preventing buffer overflow
    size_t len = strlen(modified_account.name); //size_t is a data type that represents the size of an object, it's an unsigned integer type
    modified_account.name[len-1] = '\0'; //remove newline character at the end of string

    //validate name entered
    if(!validate_name(modified_account.name))
    {
        printf("\nInvalid name.\n");
        navigate();
        return;

    }

    //validate email address entered
    printf("New email address: ");
    scanf("%s",modified_account.email_address);

    if(!validate_email(modified_account.email_address))
    {
        printf("\nInvalid email address.\n");
        navigate();
        return;

    }

    //validate mobile number entered
    printf("New mobile number: ");
    scanf("%s",modified_account.mobile);

    if(!validate_phone_number(modified_account.mobile))
    {
        printf("\nInvalid mobile number.\n");
        navigate();
        return;

    }

//Confirm if user wants to save the changes
    printf("Do you want to save?\n");
    printf("1. YES\n");
    printf("2. NO\n");
    do
    {
        printf("Enter choice 1 or 2 to continue: ");
        scanf("%s",input);
    }
    while(strcmp(input,"1")!=0&&strcmp(input,"2")!=0); //ensures that input is either 1 or 2


//save changes
    if (strcmp(input,"1")==0)
    {
   //copies modifications to the original array
    strcpy(account[i].name,modified_account.name);
    strcpy(account[i].mobile,modified_account.mobile);
    strcpy(account[i].email_address,modified_account.email_address);
  //save changes to the accounts.txt file
    save(account);
    printf("\nAccount modified succesfully.\n");
    }
    else
  //discard changes
   { printf("\nChanges are discarded.\n"); }

navigate();

}

//convert to double function
int isDouble(const char *str, double *balance)
{
    char *endptr;
    *balance = strtod(str, &endptr);

    // Check if conversion was successful
    if (*endptr != '\0') {
        // The entire string was not converted
        return 0;
    }

    return 1;
}

//transfer function
void transfer(Account account[])
{
    char acc_num_sender[MAX_LENGTH_ACCOUNT];
    char acc_num_receiver[MAX_LENGTH_ACCOUNT];
    char str[6];
    double transfer_money;
    int i;
    int j;
    char input[MAX_LENGTH_INPUT];
    char filename[MAX_FILE_NAME];
    int year=0,day=0,month=0,hour=0,min=0,sec=0;

    printf("Enter account number of sender : ");
    scanf("%s",acc_num_sender);
    printf("Enter account number of receiver : ");
    scanf("%s",acc_num_receiver);
    if(!strcmp(acc_num_sender,acc_num_receiver))
    {
        printf("\nCannot transfer money to the same account\n");
        navigate();
        return;
    }


    if(check_account_number(account,acc_num_sender)==0 || check_account_number(account,acc_num_receiver)==0 )//check if both accounts exist
        printf("Invalid account number\n");
    else
    {
        printf("Enter the amount of money to transfer\n");
        scanf("%s",str);

        if(!isDouble(str,&transfer_money) || transfer_money>10000 || transfer_money<0)//to check if the amount of money is valid
        {
            printf("Amount of money entered is invalid\nTransaction is not successful\n");
        }

        else
        {
            for(i=0; i<numberOfaccounts; i++)
            {
                if(strcmp(acc_num_sender,account[i].account_number)==0)
                {
                    if(account[i].balance<transfer_money)//check if there is enough money
                    {
                        printf("Insufficient balance\n");
                        printf("Transaction is not successful\n");
                        break;
                    }
                    else
                    {
                        printf("To confirm transaction press 1\n"); //asking user to confirm transaction
                        printf("To cancel transaction press 2\n");
                        do
                        {
                            printf("press 1 or 2 to continue: ");
                            getchar();
                            gets(input);

                        }
                        while(strcmp(input,"1")!=0 && strcmp(input,"2")!=0);// do while loop to ensure user entered 1 or 2

                        if(strcmp(input,"1")==0)
                        {
                            account[i].balance=account[i].balance-transfer_money;//update balance
                            save(account);// save changes
                            snprintf(filename,sizeof(filename),"%s.txt",account[i].account_number);
                            FILE *f2=fopen(filename,"a");// open file in append mode to write transaction
                            if (f2!=NULL) //handle error for opening file
                            {
                            TimeStamp(&year,&month,&day,&hour,&min,&sec);
                            fprintf(f2,"Transferred Amount from account: %.3lf,  Transaction ID:%05d,  TIME: %02d:%02d:%02d  %02d-%02d-%04d\n",transfer_money,transaction_id,hour,min,sec,day,month,year);
                            fclose(f2);
                            }
                            else
                                printf("Error opening file");
                            for(j=0; j<numberOfaccounts; j++)
                            {
                                if(strcmp(acc_num_receiver,account[j].account_number)==0)
                                {
                                    account[j].balance=account[j].balance+transfer_money;//update balance
                                    save(account);// save changes
                                    snprintf(filename,sizeof(filename),"%s.txt",account[j].account_number);
                                    FILE *f2=fopen(filename,"a");// open file in append mood
                                    if (f2 != NULL)//handle error for opening file
                                    {
                                    fprintf(f2,"Transferred Amount to account: %.3lf,  Transaction ID:%05d,  TIME: %02d:%02d:%02d  %02d-%02d-%04d\n",transfer_money,transaction_id++,hour,min,sec,day,month,year);//write transferred money in account_number.txt file of each account
                                    fclose(f2);
                                    }
                                    else
                                        printf("Error opening file\n");
                                    printf("The new balance of the account number %s is %.3lf\n",account[i].account_number,account[i].balance);
                                    printf("The new balance of the account number %s is %.3lf\n",account[j].account_number,account[j].balance);//print new balance
                                    break;
                                }
                            }

                            printf("\nTransaction is successful\n");
                            break;
                        }
                        else
                        {
                           navigate();
                           return;
                        }
                    }
                }
            }
        }
    }
navigate();
}

//deposit function
void deposit(Account account[])
{
    char acc_num[MAX_LENGTH_ACCOUNT];
    char str[6];
    double add_money;
    char input[MAX_LENGTH_INPUT];
    int i;
    char filename[MAX_FILE_NAME];
    int year=0,day=0,month=0,hour=0,min=0,sec=0;

    printf("Enter the account number\n");
    scanf("%s",acc_num);
    //validate account number

    if(check_account_number(account,acc_num)==0)//check account number
        printf("Invalid account number\n");
    else
    {
        printf("Enter the amount of money to deposit\n");
        scanf("%s",str);

        if(!isDouble(str,&add_money) || add_money>10000 || add_money<0) //to check if the amount of money is valid
            printf("The amount of money entered is invalid\nTransaction is not successful\n");
        else
        {
            for(i=0; i<numberOfaccounts; i++)
            {
                if(strcmp(acc_num,account[i].account_number)==0)
                {
                    printf("To confirm transaction press 1\n");
                    printf("To cancel transaction press 2\n");
                    do
                    {
                        printf("press 1 or 2 to continue: ");
                        getchar();
                        gets(input);
                    }
                    while(strcmp(input,"1")!=0 && strcmp(input,"2")!=0);// do while loop to ensure user entered 1 or 2

                    if(strcmp(input,"1")==0)
                    {
                        account[i].balance=account[i].balance+add_money;//update balance
                        save(account);
                        snprintf(filename,sizeof(filename),"%s.txt",account[i].account_number);
                        FILE *f2=fopen(filename,"a");// open file in append mood
                        if(f2 != NULL) //handle error for opening file
                        {
                        TimeStamp(&year,&day,&month,&hour,&min,&sec);
                        fprintf(f2,"Deposit:%.3lf,  Transaction ID:%05d,  TIME: %02d:%02d:%02d  %02d-%02d-%04d\n",add_money,transaction_id++,hour,min,sec,day,month,year);//write deposit money in the account_number.txt file of each account
                        fclose(f2);
                        }
                        else
                            printf("Error opening file\n");

                        printf("\nTransaction is successful\n\n");
                        printf("The new balance of the account is %.3lf\n",account[i].balance);//print new balance
                        break;
                    }
                    else
                    {
                        navigate();
                        return;
                    }
                }
            }
        }
    }
navigate();
}

//withdraw function
void withdraw(Account account[])
{
    char acc_num[MAX_LENGTH_ACCOUNT];
    char str[6];
    double withdraw_money;
    char input[MAX_LENGTH_INPUT];
    char filename[MAX_FILE_NAME];
    int i;
    int year=0,day=0,month=0,hour=0,min=0,sec=0;
    FILE *f2;


    printf("Enter the account number\n");
    scanf("%s",acc_num);

    if(check_account_number(account,acc_num)==0)//check if account exists
        printf("Invalid account number");
    else
    {
        printf("Enter the amount of money to withdraw\n");
        scanf("%s",str);

        if(!isDouble(str,&withdraw_money) || withdraw_money>10000 || withdraw_money<0)//to check if the amount of money is valid
        {
            printf("The amount of money entered is invalid\nTransaction is not successful\n");
        }

        else
        {
            for(i=0; i<numberOfaccounts; i++)
            {
                if(strcmp(acc_num,account[i].account_number)==0)
                {
                    if(account[i].balance<withdraw_money)//check if there is enough money
                    {
                        printf("Insufficient balance\n");
                        printf("Transaction is not successful\n");
                        break;
                    }
                    else
                    {
                        printf("To confirm transaction press 1\n");
                        printf("To cancel transaction press 2\n");
                        do
                        {
                            printf("press 1 or 2 to continue: ");
                            getchar();
                            gets(input);

                        }
                        while(strcmp(input,"1")!=0 && strcmp(input,"2")!=0);// do while loop to ensure user entered 1 or 2

                        if(strcmp(input,"1")==0)
                        {
                            account[i].balance=account[i].balance-withdraw_money;//update balance
                            save(account); //save changes
                            snprintf(filename,sizeof(filename),"%s.txt",account[i].account_number);
                            f2=fopen(filename,"a");//open file of the account
                            if(f2 != NULL)//handle error for opening file
                            {
                            TimeStamp(&year,&month,&day,&hour,&min,&sec);
                            fprintf(f2,"Withdrawal:%.3lf,  Transaction ID:%05d,  TIME: %02d:%02d:%02d  %02d-%02d-%04d\n",withdraw_money,transaction_id++,hour,min,sec,day,month,year);//printing withdrawn money in the file
                            fclose(f2);//closing the file
                            }
                            else
                                printf("Error in opening file\n");

                            printf("Transaction is successful\n");
                            printf("The new balance of the account is %.3lf\n",account[i].balance);//print new balance
                            break;
                        }
                        else
                        {
                            navigate();
                            return;
                        }
                    }
                }
            }
        }
    }
navigate();
}


void report() {
    char enteredAccountNum[MAX_LENGTH_ACCOUNT];
    int maxTransactionsPrint = 5;
    printf("Enter account number: ");
    scanf("%s", enteredAccountNum);

    FILE *ptr;
    ptr = fopen(strcat(enteredAccountNum, ".txt"), "r");

    if (ptr == NULL) {
        printf("Account number not found.\n");
        navigate();
    } else {

    // Count the number of transactions by counting the number of lines in the file
    int transactionCount = 0;
    char c;
    while ((c = fgetc(ptr)) != EOF) {
        if (c == '\n') {
            transactionCount++;
        }
    }

    // Move back to the beginning of the file
    fseek(ptr, 0, SEEK_SET);

       // Move to the start of the last five transactions
    int startTransaction;
     if(transactionCount > maxTransactionsPrint)
        {
            //If there are more transactions than 5 transactions,set the starting transaction to the first transaction of the last five transactions
        startTransaction=transactionCount - maxTransactionsPrint;
        }
          else
          {
        // If the total transaction count is less than or equal to 5 transactions, start from the first transaction.
            startTransaction= 0;
          }

    // Print file empty message if there are no transactions
    if (transactionCount == 0) {
        printf("\nNo transactions found.\n");
    } else {
        // Move to the starting point
        for (int i = 0; i < startTransaction; i++) {
            while ((c = fgetc(ptr)) != '\n' && c != EOF) {
                // Move to the end of the current line
            }
        }

        printf("\n\nTRANSACTIONS DETAILS:\n\n");
        // Print the transactions
        while ((c = fgetc(ptr)) != EOF) {
            putchar(c);
        }
    }

    fclose(ptr);

    navigate();
    }
}




int main()
{
    Account accounts[MAX_NUMBER_OF_ACCOUNTS]; // Create an array with type Accounts

    Load(accounts,MAX_NUMBER_OF_ACCOUNTS); // Load accounts from accounts.txt file

   loginMenu(accounts);

    return 0;
}





