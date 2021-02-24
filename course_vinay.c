//This is a c program for maintaining orders of the company Tsomato Delivery
// This program is written by Vinayak V Pastey, Dheeraj Pawar and Sushil

// Here, we are including the standard I/O library, standard library of C and the string library.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20 //This is a preprocessor that defines the value of MAX as 20.

//Some global variables
int cnt = 0;         //This counter variable keeps track of no of customers for that day.
int cnt_current = 0; //This counter variable keeps track of no of current customers for that day.

//This is a structure that stores order item details
typedef struct item_details
{
    char item_name[MAX]; //Stores Item name
    int rate;            //Stores rate of the item
    int qty;             //Stores Quantity of the item
    int amount;          //Stores the amount of the item
    struct order_details *next;
} ITEM;

//Below, is a structure of a node that defines and stores the details of customers.
typedef struct customer_details
{
    int order_no;      //Stores Order No.
    char name[MAX];    //Stores Name of the customer
    char address[MAX]; //Stores Address of the customer
    char email[MAX];   //Stores Email of the customer
    char ph_no[10];    //Stores Phone no of the customer
    ITEM *head;
    int total;  //Stores the Total amount of the bill
    int gtotal; //Stores the Grand total Incl. GST(18%)
    struct customer_details *next;
} DETAILS;

char menu[][30] = {
    "Masala Papad", "Gobi Manchuri", "Dry Gobi", "Sandwich",                                                          //Starters
    "Tomato Soup", "Chicken Chilly", "Manchow Soup",                                                                  //Soups
    "Khali Papadi", "Samosa", "Vadapav", "Mirchi", "Khanda Bhaji", "Girmit",                                          //Evening Snacks
    "North Indian Thali", "South Indian Thali", "Chicken Thali", "Mutton Thali", "Fish Thali",                        //Meals
    "Chicken Biryani", "Mutton Biryani", "Hyderabadi Biryani", "Dum Bhiryani", "Egg Biryani",                         //Rice
    "Chicken Hyderabadi", "Chicken Pepper", "Chicken Butter", "Kaju Kurma", "Chicken Khadai", "Chicken Tikka Masala", //Curries
    "Tea", "Coffee", "Mountain Dew", "Sprite", "Fanta", "Mirinda"                                                     //Liquids
};

int rates[] = {
    50, 50, 30, 60,               //Starters
    50, 80, 60,                   //Soups
    30, 20, 20, 20, 20, 30,       //Evening snacks
    130, 120, 170, 180, 200,      //Meals
    180, 220, 180, 180, 150,      //Rice
    130, 120, 140, 135, 120, 150, //Curries
    10, 15, 15, 15, 15, 15        //Drinks
};

//This function prints out the menu for the manager in order to determine item details
void print_menu()
{
    printf("///////////   Tsomato Delivery   \\\\\\\\\\\\");
    printf("\n");
    printf("                   MENU");
    printf("\n");
    printf("Starters\n");
    size_t n = sizeof(rates) / sizeof(rates[0]);
    for (int i = 0; i < n; i++)
    {
        printf("%d : %s\t\t\t\t%d\n", (i + 1), menu[i], rates[i]);
    }
}

//This function adds new customer details such as name, address, contact details and order Items
void add_customer(DETAILS **ref)
{
    int flag = 0, temp, bill_temp;
    DETAILS *new_node = (DETAILS *)malloc(sizeof(DETAILS));
    new_node->next = NULL;
    printf("Enter Name:");
    scanf("%s", new_node->name);
    printf("Enter Address:");
    scanf("%s", new_node->address);
    printf("Enter Email Id:");
    scanf("%s", new_node->email);
    printf("Enter Phone No:");
    scanf("%s", new_node->ph_no);
    new_node->total = 0;
    new_node->gtotal = 0;
    new_node->head = NULL;
    while (flag == 0)
    {
        ITEM *tempe = (ITEM *)malloc(sizeof(ITEM));
        tempe->next = NULL;
        printf("Enter the Item by ItemId:");
        scanf("%d", &temp);
        strcpy(tempe->item_name, menu[temp]);
        tempe->rate = rates[temp];
        printf("Enter Quantity:");
        scanf("%d", &temp);
        tempe->qty = temp;
        tempe->amount = tempe->qty * tempe->rate;
        new_node->total = new_node->total + tempe->amount;
        if (new_node->head == NULL)
        {
            new_node->head = tempe;
        }
        else
        {
            new_node->head->next = tempe;
        }
        printf("Do you want to continue?\nPress 0 for yes, 1 for no: ");
        scanf("%d", &flag);
    }

    bill_temp = add_gst(new_node->total);
    new_node->gtotal = bill_temp;

    if (*ref == NULL)
    {
        *ref = new_node;
    }
    else
    {
        DETAILS *last = *ref;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new_node;
    }
    return;
}

int add_gst(int temp)
{
    int x = temp * 0.18;
    return temp + x;
}

void del_customer(DETAILS **head_ref, int orderID)
{
    DETAILS *temp = *head_ref, *prev;

    if (temp != NULL && temp->order_no == orderID)
    {
        *head_ref = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->order_no != orderID)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return;
    }
    prev->next = temp->next;
    free(temp);
    return;
}

void print_cust_bill(DETAILS *head_ref, int orderID)
{
    DETAILS *temp = head_ref;

    if (temp != NULL && temp->order_no == orderID)
    {
        printf("Order Number: %d\n", temp->order_no);
        printf("Name: %s\n", temp->name);
        printf("Address: %s\n", temp->address);
        printf("Email: %s\n", temp->email);
        printf("Phone No: %s\n", temp->ph_no);
        //Add no of orders
        printf("INVOICE\n");
        printf("Si no.  Name\t\t\t\tRate  Qty  Amount\n");
        int i = 1;
        ITEM *tempe = temp->head;
        while (tempe != NULL)
        {
            printf("%d\t%s\t\t\t\t%d  %d  %d\n", i, tempe->item_name, tempe->rate, tempe->qty, tempe->amount);
            tempe = tempe->next;
            i++;
        }
        printf("Total = %d\n", temp->total);
        printf("Tax = %d\n", (temp->total * 0.18));
        printf("Grand Total Total = %d\n", temp->gtotal);
        return;
    }

    while (temp != NULL && temp->order_no != orderID)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {

        return;
    }
    printf("Order Number: %d\n", temp->order_no);
    printf("Name: %s\n", temp->name);
    printf("Address: %s\n", temp->address);
    printf("Email: %s\n", temp->email);
    printf("Phone No: %s\n", temp->ph_no);
    //Add no of orders
    printf("INVOICE\n");
    printf("Si no.  Name\t\t\t\tRate  Qty  Amount\n");
    int i = 1;
    ITEM *tempe = temp->head;
    while (tempe != NULL)
    {
        printf("%d\t%s\t\t\t\t%d  %d  %d\n", i, tempe->item_name, tempe->rate, tempe->qty, tempe->amount);
        tempe = tempe->next;
        i++;
    }
    printf("Total = %d\n", temp->total);
    printf("Tax = %d\n", (temp->total * 0.18));
    printf("Grand Total Total = %d\n", temp->gtotal);
    return;
}

void print_cust_details(DETAILS *head_ref, int orderID)
{
    DETAILS *temp = head_ref;

    if (temp != NULL && temp->order_no == orderID)
    {
        printf("Order Number: %d\n", temp->order_no);
        printf("Name: %s\n", temp->name);
        printf("Address: %s\n", temp->address);
        printf("Email: %s\n", temp->email);
        printf("Phone No: %s\n", temp->ph_no);

        //Add no of orders
        return;
    }

    while (temp != NULL && temp->order_no != orderID)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {

        return;
    }
    printf("Order Number: %d\n", temp->order_no);
    printf("Name: %s\n", temp->name);
    printf("Address: %s\n", temp->address);
    printf("Email: %s\n", temp->email);
    printf("Phone No: %s\n", temp->ph_no);
    //Add no of orders
    return;
}

void fprint_bill(DETAILS *head_ref, int orderID)
{
    FILE *cust_bill;
    DETAILS *temp = head_ref;

    // fprintf(fptr,"%s\n%s\n%s\nOrder Items:\n",head->name,head->address,head->ph_no);
    // fclose(fptr);
    

    if (temp != NULL && temp->order_no == orderID)
    {
        
        cust_bill= fopen("hello.txt","w");
        printf("Order Number: %d\n", temp->order_no);
        printf("Name: %s\n", temp->name);
        printf("Address: %s\n", temp->address);
        printf("Email: %s\n", temp->email);
        printf("Phone No: %s\n", temp->ph_no);
        //Add no of orders
        printf("INVOICE\n");
        printf("Si no.  Name\t\t\t\tRate  Qty  Amount\n");
        int i = 1;
        ITEM *tempe = temp->head;
        while (tempe != NULL)
        {
            printf("%d\t%s\t\t\t\t%d  %d  %d\n", i, tempe->item_name, tempe->rate, tempe->qty, tempe->amount);
            tempe = tempe->next;
            i++;
        }
        printf("Total = %d\n", temp->total);
        printf("Tax = %d\n", (temp->total * 0.18));
        printf("Grand Total Total = %d\n", temp->gtotal);
        return;
    }

    while (temp != NULL && temp->order_no != orderID)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {

        return;
    }
    printf("Order Number: %d\n", temp->order_no);
    printf("Name: %s\n", temp->name);
    printf("Address: %s\n", temp->address);
    printf("Email: %s\n", temp->email);
    printf("Phone No: %s\n", temp->ph_no);
    //Add no of orders
    printf("INVOICE\n");
    printf("Si no.  Name\t\t\t\tRate  Qty  Amount\n");
    int i = 1;
    ITEM *tempe = temp->head;
    while (tempe != NULL)
    {
        printf("%d\t%s\t\t\t\t%d  %d  %d\n", i, tempe->item_name, tempe->rate, tempe->qty, tempe->amount);
        tempe = tempe->next;
        i++;
    }
    printf("Total = %d\n", temp->total);
    printf("Tax = %d\n", (temp->total * 0.18));
    printf("Grand Total Total = %d\n", temp->gtotal);
    return;
}

int main(void)
{
    print_menu();
    DETAILS *head = NULL;
    int choice, exit_flag = 0;
    while (exit_flag == 0)
    {
        printf("1: ADD CUSTOMER\n");
        printf("2: DELETE CUSTOMER\n");
        printf("3: PRINT CUSTOMER DETAILS\n");
        printf("4: PRINT CUSTOMER BILL\n");
        printf("5: PRINT ORDER STATUS OF CUSTOMER\n");
        printf("5: EXIT THE APPLICATION\n");
        switch (choice)
        {
        case 1:
            add_customer(&head);
            break;

        case 2:
            printf("Enter orderID of customer:- ");
            int temp;
            scanf("%d", &temp);
            printf("DELETING CUSTOMER ORDER AND DETAILS......\n");
            del_customer(&head, temp);
            break;

        case 3:
            printf("Enter orderID of customer:- ");
            int temp;
            scanf("%d", &temp);
            printf("PRINTING CUSTOMER DETAILS.....\n");
            print_cust_details(head, temp);
            break;

        case 4:
            printf("Enter orderID of customer:- ");
            int temp;
            scanf("%d", &temp);
            printf("PRINTING CUSTOMER DETAILS AND BILL.....\n");
            print_cust_bill(head, temp);
            printf("Do you want to print it in a sepearte file?\nyes-1 no-0  :");
            int yes = 0;
            scanf("%d",&yes);
            if(yes == 1){
                printf("PRINTING BILL INTO A TEXT FILE......");
                fprint_bill(head, temp);
            }
            break;

        case 5:
            printf("Enter orderID of customer:- ");
            int temp;
            scanf("%d", &temp);
            check_status(&head, temp);
            break;

        case 6:
            printf("EXITING THE APPLICATION........");
            exit_flag = 1;
            break;
        default:
            printf("Enter Valid Option\n");
            break;
        }
    }
    if (exit_flag == 1)
    {
        return 0;
    }
    // FILE *fptr;
    // fptr = fopen("hello.txt","w");
    // fprintf(fptr,"%s\n%s\n%s\nOrder Items:\n",head->name,head->address,head->ph_no);
    // fclose(fptr);
}