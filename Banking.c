/*******************************************************************************
 * List header files - do NOT use any other header files. Note that stdlib.h is
 * included in case you want to use any of the functions in there. However the
 * task can be achieved with stdio.h and string.h only.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "rle.h"
/*******************************************************************************
 * List preprocessing directives - you may define your own.
*******************************************************************************/
#define MAX_NUMBER_CUSTOMERS 10
#define MAX_NAME_LENGTH 10
#define MAX_NUMBER_ACOUNT 3
#define MALE 0
#define FEMALE 1
#define TRUE 1
#define FALSE 0
#define DB_NAME "bank_database"
#define RLE_DB_NAME "rle_bank_database"
#define ENTER 10
/*******************************************************************************
 * List structs - you may define struct date_time and struct flight only. Each
 * struct definition should have only the fields mentioned in the assignment
 * description.
*******************************************************************************/
/* This structure holds a customers account information*/
typedef struct account{
    double balance;
    int account_code;
}account_t;

/*This structure creates a date of birth object*/
typedef struct dob{
    int day;
    int month;
    int year;
}dob_t;

/*This structure holds all details about a customer and is the base struct*/
typedef struct customer{
    char firstname[MAX_NAME_LENGTH + 1];
    char surname[MAX_NAME_LENGTH + 1];
    dob_t dob;
    int post_code;
    account_t account;
    int customer_id;
}customer_t;

/*******************************************************************************
 * Function prototypes - Need to fix the return types and arguments passed
*******************************************************************************/
int user_login(void);/*Zach*/
int check_password(char password[14]);
int add_customer(customer_t customers[], int customer_count);/*Zach*/
int transfer_money(customer_t customers[], int customer1_index, int customer2_index);/*Jerry, Wally*/
int withdraw_money(customer_t customers[], int customer_index);/*Jerry, Wally*/
int deposit_money(customer_t customers[], int customer_index);/*Jerry, Wally*/
int user_logout(void);/*Neha, Zach can help if needed*/
void check_balance();/*Jerry, Wally*/
void update_customer_details();/*Jerry, Wally*/
void view_all_customers(const customer_t customers[], int customer_count);/*Zach*/
int select_customer(const customer_t customers[], int customer_count);/*Zach*/
void save_database(const customer_t customers[], int customer_count); /*Zach*/
int compress_database(void);/*Zach*/
int decompress_database(void);/*Zach*/
int load_database(customer_t customers[]);/*Zach*/
void print_main_menu(void);/*Zach*/
int get_user_menu_choice(void);/*Zach*/
void login_banner(void);/*Zach*/
int clear(void);/*Zach*/
int generate_account_code(const customer_t customers[], int customer_count);/*Zach*/
void print_customer_menu(void);/*Zach*/
int customer_main(customer_t customers[], int customer_count);
void customer_select(customer_t customers[], int customer_count);/*Zach*/
/*******************************************************************************
 * Main
*******************************************************************************/
int main(void){
	customer_t customers[MAX_NUMBER_CUSTOMERS];
	int login_success, menu_choice, number_customers;
    system("clear");
	login_banner();
	login_success = user_login();
	    while(login_success == TRUE){
        print_main_menu();
        menu_choice = get_user_menu_choice();
        switch(menu_choice){
            case 1:
            	clear();
                number_customers = add_customer(customers, number_customers);
                break;
            case 2:
                customer_select(customers, number_customers);
                break;
            case 3:
                clear();
                view_all_customers(customers, number_customers);
                system("clear");
                break;
            case 4:
                save_database(customers, number_customers);
                break;
            case 5:
                number_customers = load_database(customers);
                break;
            case 6:
            	login_success = FALSE;
            	break;
            default :
                printf("Invalid choice\n");
                break;
            }
        }
	return 0;
}

/*******************************************************************************
 * This function prints the initial menu with program options.
 * inputs:
 * - none
 * outputs:
 * - none
 *Author: Zach
*******************************************************************************/
void print_main_menu (void)
{
    printf(
    "1. Add Customer\n"
    "2. Select Customer\n"
    "3. Show all Customers\n"
    "4. Save Database\n"
    "5. Load Database\n"
    "6. Logout\n"
    "Enter choice (number between 1-6)>\n");
}

/*******************************************************************************
*Takes input from the user and returns a signed integer
*Inputs: None
*Outputs: Integer representing the users choice
*Author: Zach
*******************************************************************************/
int get_user_menu_choice(void){
    int choice;
    scanf("%d", &choice);
    return choice;
}

/*******************************************************************************
*Function is designed to clear the stdin buffer.
*Inputs: None
*Outputs: Null character '\0'
*Author: Zach
*******************************************************************************/
int clear(void){
    int c;
    while ((c = getchar()) != EOF && c != '\n')
        continue;
    return c;
}
/*******************************************************************************
*Prints out a warning banner
*Inputs: None
*Outputs: None
*Author: Zach
*******************************************************************************/
void login_banner(){
	int i, width = 80;
	for(i=0;i<width;++i){
		printf("\x1b[31m#\x1b[0m");
	}
	printf("\n\x1b[31m#\x1b[0m");
	for(i=0;i<29;++i){
		printf(" ");
	}
	printf("Welcome to Tech Bank");
	for(i=0;i<29;++i){
		printf(" ");
	}
	printf("\x1b[31m#\x1b[0m\n\x1b[31m#\x1b[0m");
	for(i=0;i<18;++i){
		printf(" ");
	}
	printf("All connections are monitored and recorded");
	for(i=0;i<18;++i){
		printf(" ");
	}
	printf("\x1b[31m#\x1b[0m\n\x1b[31m#\x1b[0m");
	for(i=0;i<4;++i){
		printf(" ");
	}
	printf("Disconnect IMMEDIATELY if you're not authorised to access this system!");
	for(i=0;i<4;++i){
		printf(" ");
	}
	printf("\x1b[31m#\x1b[0m\n");
		for(i=0;i<width;++i){
		printf("\x1b[31m#\x1b[0m");
	}
	printf("\n");
}

/*******************************************************************************
*Gets password from staff and if 3 incorect trys closes application
*Inputs: Password
*Outputs: Int
*Author: Zach
*******************************************************************************/
int user_login(void){
	int fail_count = 0;
	char password[14];
	while(fail_count != 3){
		printf("Password: ");
		scanf("%s", password);
		if(check_password(password)!=1){
			++fail_count;
			if(fail_count==3){
				exit(0);
			}else{
				printf("Invaild Password %d attempts remaining.\n", 3-fail_count);
			}
		}else{
			printf("Login Success\n");
            system("sleep 2");
            system("clear");
			return TRUE;
		}
	}
	return FALSE;
}

/*******************************************************************************
*Checks the supplied passsword
*Inputs: Password "Password123"
*Outputs: Int, represents success or failure
*Author: Zach
*******************************************************************************/
int check_password(char password[14]){
	char key[] = "FundamentalsOfC";
	char xor[strlen(password)];
    /*precomputed xor valus of the password*/
	int DB_HASH[] = {22, 20, 29, 23, 22, 2, 23, 10, 69, 83, 95, 4, 1};
	int i, status = 0;
	for(i=0;i<strlen(password);i++){
		xor[i] = (char)(password[i] ^ key[i]);
	}
	for(i=0;i<strlen(xor);++i){
        /*compares each character to its expected value*/
		if(xor[i] != DB_HASH[i]){
			break;
		}else{
			++status;
		}
	}
    /*ensures that the correct ammount of characters wer input*/
	if(status == 11){
		return TRUE;
	}else{
		return FALSE;
	}
}

/*******************************************************************************
*add customer detail to the database
*Inputs: first name, last name, DOB, account balance, sex, postcode, account code
*Outputs: None
*Author: Zach
*******************************************************************************/
int add_customer(customer_t customers[], int customer_count){
    int s = 0, day, month, year, postcode, account_code; /*Sentinel value*/
    char answer[2];
    /*check to make sure there is space to add additonal customers*/
    if(customer_count == MAX_NUMBER_CUSTOMERS){
        printf("Cannot add more customers - memory full\n");
        return customer_count;
    }else{
        system("clear");
        /*gets customer first name and checks to make sure it is within the
        length upper bound*/
        while(s==0){
            printf("Please enter customer firstname>\n");
            scanf("%s",customers[customer_count].firstname);
            if(strlen(customers[customer_count].firstname) > MAX_NAME_LENGTH){
                clear();
                printf("Input exceeded maximum length\n");
            }else{
                s=1;
            }
        }
        /*gets customer surnname and checks to make sure it is within the
        length upper bound*/
        while(s==1){
            printf("Please enter customer surname>\n");
            scanf("%s",customers[customer_count].surname);
            if(strlen(customers[customer_count].surname) > MAX_NAME_LENGTH){
                clear();
                printf("Input exceeded maximum length\n");
            }else{
                s=2;
            }
        }
        while(s==2){
            printf("Please enter the customers birthday separated by spaces\n");
            printf("Use the following format day, month year>\n");
            scanf("%d %d %d",&day, &month, &year);
            clear();
            /*Checks for a valid date*/
            if(month< 1 || month > 12){printf("Invalid input\n");}
            else if(day < 1 || day> 31){printf("Invalid input\n");}
            else if(year < 1870 || year > 2019){printf("Invalid input\n");}
            else{s=3;
                customers[customer_count].dob.day = day;
                customers[customer_count].dob.month = month;
                customers[customer_count].dob.year = year;}
        }
        while(s==3){
            printf("Please enter postcode>\n");
            scanf("%d", &postcode);
            /*Checks for a valid postcode, taken from
            https://en.wikipedia.org/wiki/Postcodes_in_Australia*/
            if((postcode > 199 && postcode < 300) || (postcode > 799 &&
                postcode < 6798) || (postcode > 6999 && postcode < 10000)){
                s = 4;
                customers[customer_count].post_code = postcode;
            }else{
                printf("Invalid input\n");
            }
            clear();
        }
        account_code = generate_account_code(customers, customer_count);
        customers[customer_count].account.account_code = account_code;
        customers[customer_count].customer_id = customer_count + 500;
        printf("Assigned %s, customer ID: %d and opened initial account with account code: %d\n",
            customers[customer_count].firstname, customers[customer_count].customer_id, account_code);
        printf("Would you like to add an initial deposit? (y/n)\n");
        scanf("%c", answer);
        if(strcmp(answer, "y") == 0){
            printf("Enter deposit ammount: $");
            scanf("%lf", &customers[customer_count].account.balance);
        }else{
            customers[customer_count].account.balance = 0;
        }
    }
    system("clear");
    return ++customer_count;
}

/*******************************************************************************
*Generates a unique account number
*Inputs: None
*Outputs: unique account code
*Author: Zach
*******************************************************************************/
int generate_account_code(const customer_t customers[], int customer_count){
    int i,count=10000;
    if(customer_count == 0){
        return count;
    }else{
        for(i=0;i<customer_count;++i){
            count += 3;
        }
    }
    return count;
}

/*******************************************************************************
*Saves the customers array to disk NOTE: will need to fix how accounts are saved
*Inputs: None
*Outputs: Compressed DB
*Author: Zach
*******************************************************************************/
void save_database(const customer_t customers[], int customer_count){
    FILE * fp;
    int i,status;
    fp = fopen(DB_NAME, "w");
    /*Iterates through array of flights and saves each flight to a the database
    *file seperated by '\n' whilst elements are space seperated*/
    for(i = 0; i < customer_count; ++i){
        fprintf (fp, "%s %s %d %d %d %d %d %lf %d\n", customers[i].firstname,
            customers[i].surname,
            customers[i].dob.day,
            customers[i].dob.month,
            customers[i].dob.year,
            customers[i].post_code,
            customers[i].account.account_code,
            customers[i].account.balance,
            customers[i].customer_id);
    }
    fclose(fp);
    status = compress_database();
    if(status == 1){
        printf("\033[32mSaved database successfully\033[0m\n");
    }else{
        printf("\033[31mFailed to save database\033[0m\n");
    }
    system("sleep 1");
    system("clear");
}

/*******************************************************************************
*Compresses the DB file using RLE
*Inputs: None
*Outputs: status of cCompressed DB
*Author: Zach
*******************************************************************************/
int compress_database(void){
    int status;
    FILE *input_file, *output_file;
    input_file = fopen(DB_NAME, "r");
    output_file = fopen(RLE_DB_NAME, "w");

    status = run_length_encode(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    remove(DB_NAME);
    return status;
}

/*******************************************************************************
*de-compress the DB file using RLE
*Inputs: Compressed DB
*Outputs: status of decompression
*Author: Zach
*******************************************************************************/
int decompress_database(void){
    int status;
    FILE *input_file, *output_file;
    input_file = fopen(RLE_DB_NAME, "r");
    output_file = fopen(DB_NAME, "w");

    status = run_length_decode(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    return status;
}

/*******************************************************************************
*Reads an existing Database file, if one is found the current customers
*are cleared and the ones from the file are loaded into memory
*Inputs: Array of customers
*Outputs: Number of customers added to the array
*Author: Zach
*******************************************************************************/
int load_database(customer_t customers[]){
    FILE * fp;
    int status, i=0;

    /*calls the decompression function and assigns the return to status*/
    status = decompress_database();
    /*if status is 1 than the database has been succefully loaded*/
    if(status == 1){
        printf("\033[32mLoaded database successfully\033[0m\n");
    }else{
        printf("\033[31mFailed to load database\033[0m\n");
    }
    system("sleep 1");
    system("clear");

    fp = fopen(DB_NAME, "r");
    /*Checks that the Database actually exists*/
    if (fp == NULL){
      printf("Read error\n");
    }else{
        /*Clears the existing array of customers stored in memory*/
        memset((char*)customers, '\0', sizeof(customer_t));
        /*Reads the assigns the values to their correct elemement*/
        while( fscanf( fp, "%s %s %d %d %d %d %d %lf %d",
            customers[i].firstname,
            customers[i].surname,
            &customers[i].dob.day,
            &customers[i].dob.month,
            &customers[i].dob.year,
            &customers[i].post_code,
            &customers[i].account.account_code,
            &customers[i].account.balance,
            &customers[i].customer_id) !=EOF
            && i < MAX_NUMBER_CUSTOMERS){
        ++i;
         }
     }
    fclose(fp);
    remove(DB_NAME);
    return i;
}

/*******************************************************************************
*Prints all customer information
*Inputs: Array of customers
*Outputs:
*Author: Zach
*******************************************************************************/
void view_all_customers(const customer_t customers[], int customer_count){
    int i,j,s=0;

    printf("ID  Firstname  Surname    DOB        PC   ACT   Balance   \n");
    printf("___ __________ __________ __________ ____ _____ __________\n");
    for(i=0;i<customer_count;++i){
        printf("%d ",customers[i].customer_id);
        printf("%s", customers[i].firstname);
        /*ensures correct padding is added to align based of the strings length*/
        for(j=0; j < (11 - strlen(customers[i].firstname));++j){
            printf(" ");
        }
        printf("%s", customers[i].surname);
        /*ensures correct padding is added to align based of the strings length*/
        for(j=0; j < (11 - strlen(customers[i].surname));++j){
            printf(" ");
        }
        /*prints a leading 0 if needed*/
        printf("%02d-%02d-%d ",customers[i].dob.day,
            customers[i].dob.month, customers[i].dob.year);
        /*prints a leading 0 if needed*/
        printf("%04d ",customers[i].post_code);
        printf("%d ", customers[i].account.account_code);
        /*prints account balance in either red or green depending if it is
        positive or negative*/
        if(customers[i].account.balance > 0){
            printf("\033[32m%.2f\033[0m\n",customers[i].account.balance);
        }else{
            printf("\033[31m%.2f\033[0m\n",customers[i].account.balance);
        }
    }
    /*Implements a wait component so user can move on when ready*/
    printf("Please press 'enter' when done\n");
    while(s==0){
        if(getchar() == ENTER){
            s=1;
        }
    }
}

/*******************************************************************************
*Acts a a decision point for bank staff to proceed down interacting with a
*customer account execution path.
*Inputs: Array of customers
*Outputs:
*Author: Zach
*******************************************************************************/
void customer_select(customer_t customers[], int customer_count){
    int s = 0;
    char op_char[2];

    system("clear");
    view_all_customers(customers, customer_count);
    clear();
    while(s==0){
        printf("Would you like to operate on a customer (y/n)?\n");
        scanf("%c", op_char);
        /*checks users response and if affirmitve proceeds else ends execution*/
        if(strcmp(op_char, "y") == 0){
            customer_main(customers, customer_count);
        }else if(strcmp(op_char, "n") == 0){
            s=1;
            system("clear");
            break;
        }else{
            printf("Invalid Input\n");
        }
        clear();
    }
}

/*******************************************************************************
*Implements the customer menu and possible options
*Inputs: array of customers
*Outputs: N/A
*Author: Zach
*******************************************************************************/
int customer_main(customer_t customers[], int customer_count){
    int menu_choice= 0, cx1_loc, cx2_loc;
    /*assigns the the selected customer to this variable*/
    cx1_loc = select_customer(customers, customer_count);
    while(menu_choice != 4){
        print_customer_menu();
        menu_choice = get_user_menu_choice();
            switch(menu_choice){
                /*for all cases if 1 is return success is printed in green else
                failure in red*/
                case 1:
                    clear();
                    if(withdraw_money(customers, cx1_loc) == 1){
                        printf("\033[32mWithdrawal Success\033[0m\n");
                    }else{
                        printf("\033[31mWithdrawal Failed\033[0m\n");
                    }
                    break;
                case 2:
                    /*Assigns the transfer recipients index location*/
                    cx2_loc = select_customer(customers, customer_count);
                    if(transfer_money(customers, cx1_loc, cx2_loc) == 1){
                        printf("\033[32mTransfer Success\033[0m\n");
                    }else{
                        printf("\033[31mTransfer Failed\033[0m\n");
                    }
                    break;
                case 3:
                    if(deposit_money(customers, cx1_loc) == 1){
                        printf("\033[32mDeposit Success\033[0m\n");
                    }else{
                        printf("\033[31mDeposit Failed\033[0m\n");
                    }
                    break;
                case 4:
                    break;
                default :
                    printf("Invalid choice\n");
                    break;
                }
        }
    return 1;
}

/*******************************************************************************
*Provides a method to select a customer based on the customers ID
*Inputs: array of customers and length of arroat
*Outputs: index location of the customer
*Author: Zach
*******************************************************************************/
int select_customer(const customer_t customers[], int customer_count){
    int s = 0, cx_id, cx_match, cx_loc, i;
    /*ensures a valid ID is given and will loop until one is*/
    while(s==0){
        printf("Please enter customer ID: ");
        scanf("%d", &cx_id);
        for(i=0;i<customer_count;++i){
                if(cx_id == customers[i].customer_id){
                    cx_match = TRUE;
                    /*when a match is found assigns the position to cx_loc*/
                    cx_loc = i;
                    break;
                }
            }
        if(cx_match == TRUE){
            s=1;
        }else{
            printf("Invalid Customer ID please try again\n");
        }
    }
    return cx_loc;
}

/*******************************************************************************
*Withdraw money from a customer account
*Inputs: array of customers and the index the selected customer is
*Outputs: changed balance
*Author: Zach
*******************************************************************************/
int withdraw_money(customer_t customers[], int customer_index){
    double w_ammount;/*withdrawal ammount*/

    printf("Please input ammount to be withdraw from customers account>\n"
        "$");
    scanf("%lf", &w_ammount);
    /*Check to make sure the customer isnt trying to over draw account*/
    if(w_ammount <= customers[customer_index].account.balance){
        customers[customer_index].account.balance -= w_ammount;
        printf("Account balance is now: $%.2f\n", customers[customer_index].account.balance);
        return 1;
    }else{
        printf("\033[31mInsufficient funds in customer account\033[0m\n");
        return -1;
    }
}

/*******************************************************************************
*Deposit money to a customer account
*Inputs: array of customers and the index the selected customer is
*Outputs: changed balance
*Author: Zach
*******************************************************************************/
int deposit_money(customer_t customers[], int customer_index){
    double d_ammount; /*Deposit ammount*/

    printf("Please input ammount to be deposited into the customers account>\n"
        "$");
    scanf("%lf", &d_ammount);
    /*Ensure this is in fact a deposit not a withdrawal or null operation*/
    if(d_ammount > 0){
        customers[customer_index].account.balance += d_ammount;
        printf("Account balance is now: $%.2f\n", customers[customer_index].account.balance);
        return 1;
    }else{
        printf("\033[31mInvalid value entered\033[0m\n");
        return -1;
    }
}

/*******************************************************************************
*Transfer money between customers accounts
*Inputs: array of customers and the index the selected customers are
*Outputs: Integer that represents the status of the transaction
*Author: Zach
*******************************************************************************/
int transfer_money(customer_t customers[], int cx1_index, int cx2_index){
    double t_ammount; /*Ammount to be transferred*/

    printf("Please input ammount to be transferred>\n"
        "$");
    scanf("%lf", &t_ammount);
    /*Check to make sure a positve ammount is transferred to stop a reverese
    transaction occouring and also that the customer has enough money to make
    the transfer*/
    if(t_ammount > 0 && t_ammount <= customers[cx1_index].account.balance){
        customers[cx1_index].account.balance -= t_ammount;
        customers[cx2_index].account.balance += t_ammount;
        return 1;
    /*If the ammount to be transfered is more than the balance print correct
    error*/
    }else if(t_ammount > customers[cx1_index].account.balance){
        printf("\033[31mInsufficient funds in account\033[0m\n");
        return -1;
    }else{
        printf("\033[31mInvalid value entered\033[0m\n");
        return -1;
    }
}
/*******************************************************************************
*Prints customer menu
*Inputs: N/A
*Outputs: N/A
*Author: Zach
*******************************************************************************/
void print_customer_menu(void){
    printf("\n"
    "1. Withdraw\n"
    "2. Transfer\n"
    "3. Deposit\n"
    "4. Back\n"
    "Enter choice (number between 1-4)>\n");
}
