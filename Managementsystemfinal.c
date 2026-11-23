/*
 BASIC FEATURES:
 1 - Package Operations -> Add, display, search, update, delete tour packages.
 2 - Customer Operations -> Register, view, delete customer records.
 3 - Tour Booking & Billing -> Select customer & package, apply transport discounts & 9% tax, print bill.
 4 - Admin Portal -> Password-protected view of all customers and packages.
 5 - File Handling -> Auto-save and load packages and customer records.
 6 - Input Validation -> Range checks, duplicate ID prevention, and text validation.

 ADDITIONAL FEATURES:
 1 - Budget-Friendly Package Finder
 2 - Transport-Based Discounts (Train: 5%, Aeroplane: 10%, Bus: 3%)
 3 - Accommodation Selection (Budget, Standard, Luxury)
*/

#include <stdio.h>
#include <string.h>

#define MAX 100
#define ADMIN_PASS "4021"

typedef struct Tour
{
    int packageID;
    char destination[50];
    int days;          // Package duration
    int transport;     // 1-Train, 2-Aeroplane, 3-Bus
    int accommodation; // 1-Budget, 2-Standard, 3-Luxury
    float price;       // Price per person
} Tour;

typedef struct
{
    int customerID;
    char name[50];
    char phone[20];
} customer;

void clearInput(void);
int getInt(int min, int max);
float getPrice(void);
void getText(char text[]);

void addPackage(Tour[], int *count);
void displayPackages(Tour[], int count);
void searchPackage(Tour[], int count);
void updatePackage(Tour[], int count);
void deletePackage(Tour[], int *count);

int findCustomer(customer customers[], int customerCount, int id);
void RegisterCustomer(customer customers[], int *customerCount);
void viewCustomer(customer customers[], int customerCount);
void deleteCustomer(customer customers[], int *customerCount);

float CalculateTotal(Tour *packages, int members, float *discount, float *tax);
void BudgetFriendly(Tour packages[], int count);
void generateBill(Tour packages[], int count, customer customers[], int customerCount);
void adminViewAll(customer customers[], int customerCount, Tour packages[], int count);

int saveData(Tour[], int count);
void loadData(Tour[], int *count);
int saveCustomers(customer customers[], int customerCount);
void loadCustomers(customer customers[], int *customerCount);

int main(void)
{
    Tour packages[MAX]; // array capable of holding 100 packages
    customer customers[MAX];
    int count = 0;
    int customerCount = 0;

    loadData(packages, &count);
    loadCustomers(customers, &customerCount);

    int choice;

    do
    {
        printf("\n======= PAKISTAN TOUR AND TRAVEL MANAGEMENT SYSTEM =======\n");
        printf("1.  Add Tour Package\n");
        printf("2.  Display All Packages\n");
        printf("3.  Search Package\n");
        printf("4.  Update Package\n");
        printf("5.  Delete Package\n");
        printf("6.  Register Customer\n");
        printf("7.  View Customer Details\n");
        printf("8.  Delete Customer\n");
        printf("9.  Budget-Friendly Package Finder\n");
        printf("10. Book Tour & Generate Bill\n");
        printf("11. View All Customers & Packages (Admin)\n");
        printf("12. Save and Exit\n");

        printf("Enter your choice (1-12): ");
        choice = getInt(1, 12);

        switch (choice)
        {
        case 1:
            addPackage(packages, &count);
            break;
        case 2:
            displayPackages(packages, count);
            break;
        case 3:
            searchPackage(packages, count);
            break;
        case 4:
            updatePackage(packages, count);
            break;
        case 5:
            deletePackage(packages, &count);
            break;
        case 6:
            RegisterCustomer(customers, &customerCount);
            break;
        case 7:
            viewCustomer(customers, customerCount);
            break;
        case 8:
            deleteCustomer(customers, &customerCount);
            break;
        case 9:
            BudgetFriendly(packages, count);
            break;
        case 10:
            generateBill(packages, count, customers, customerCount);
            break;
        case 11:
            adminViewAll(customers, customerCount, packages, count);
            break;
        case 12:
            if (saveData(packages, count))
                printf("Packages saved successfully!\n");
            else
                printf("Packages could not be saved!\n");

            if (saveCustomers(customers, customerCount))
                printf("Customers saved successfully!\n");
            else
                printf("Customers could not be saved!\n");

            printf("Goodbye!\n");
            break;
        }
    } while (choice != 12);

    return 0;
}

/*----------- Clear leftover characters from input buffer -----------*/
void clearInput(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

/*----------- Validates integer input within a given range -----------*/
int getInt(int min, int max)
{
    int num;
    char ch;
    while (1)
    {
        if (scanf("%d", &num) != 1)
        {
            printf("Invalid input! Enter an integer: ");
            clearInput();
        }
        else
        {
            ch = getchar();
            while (ch == ' ')
                ch = getchar();
            if (ch != '\n')
            {
                printf("Invalid input! Enter again: ");
                clearInput();
            }
            else if (num < min || num > max)
                printf("Invalid choice! Enter again (%d-%d): ", min, max);
            else
                return num;
        }
    }
}

/*----------- Validates text input to allow only letters and spaces -----------*/
void getText(char text[])
{
    int i;
    do
    {
        scanf(" %49[^\n]", text);
        clearInput();
        for (i = 0; text[i] != '\0'; i++)
        {
            if ((text[i] < 'A' || text[i] > 'Z') &&
                (text[i] < 'a' || text[i] > 'z') &&
                text[i] != ' ')
            {
                break;
            }
        }
        if (text[i] != '\0')
            printf("Invalid input! Only letters and spaces are allowed: ");
    } while (text[i] != '\0');
}

/*----------- Validates positive float input for price -----------*/
float getPrice(void)
{
    float price;
    char ch;
    while (1)
    {
        if (scanf("%f", &price) != 1)
        {
            printf("Invalid price! Enter again: ");
            clearInput();
        }
        else
        {
            ch = getchar();
            if (ch != '\n')
            {
                printf("Invalid input! Enter again: ");
                clearInput();
            }
            else if (price <= 0)
                printf("Price must be greater than zero! Enter again: ");
            else
                return price;
        }
    }
}

/*----------- To add packages and check for duplicate IDs -----------*/
void addPackage(Tour packages[], int *count)
{
    char ch;
    if (*count >= MAX)
    {
        printf("Package storage is full!\n");
        return;
    }

    do
    {
        int i = *count;
        int duplicate;
        printf("\n=== Add Tour Package ===\n");

        do
        {
            duplicate = 0;
            printf("Package ID: ");
            packages[i].packageID = getInt(1, 999999);

            for (int j = 0; j < *count; j++)
            {
                if (packages[i].packageID == packages[j].packageID)
                {
                    printf("ID already exists! Enter again.\n");
                    duplicate = 1;
                    break;
                }
            }
        } while (duplicate);

        printf("Destination: ");
        getText(packages[i].destination);

        printf("Days: ");
        packages[i].days = getInt(1, 365);

        printf("Transport:\n 1- Train (5%% Discount)\n 2- Aeroplane (10%% Discount)\n 3- Bus (3%% Discount)\nChoice: ");
        packages[i].transport = getInt(1, 3);

        if (packages[i].transport == 1)
            printf("Mode Of Transport: Train\n");
        else if (packages[i].transport == 2)
            printf("Mode Of Transport: Aeroplane\n");
        else
            printf("Mode Of Transport: Bus\n");

        printf("Accommodation:\n 1- Budget Hotel\n 2- Standard Hotel\n 3- Luxury Hotel\nChoice: ");
        packages[i].accommodation = getInt(1, 3);

        if (packages[i].accommodation == 1)
            printf("Accommodation: Budget Hotel\n");
        else if (packages[i].accommodation == 2)
            printf("Accommodation: Standard Hotel\n");
        else
            printf("Accommodation: Luxury Hotel\n");

        printf("Price per person (Rs.): ");
        packages[i].price = getPrice();

        (*count)++;
        saveData(packages, *count);
        printf("Package added and saved successfully!\n");

        if (*count >= MAX)
        {
            printf("Storage is full!\n");
            break;
        }

        printf("Do you want to add more packages? (Y/N): ");
        scanf(" %c", &ch);
        clearInput();
    } while (ch == 'Y' || ch == 'y');
}

/*----------- Display all available tour packages -----------*/
void displayPackages(Tour packages[], int count)
{
    if (count == 0)
    {
        printf("No packages available!\n");
        return;
    }

    printf("\n=== AVAILABLE TOUR PACKAGES ===\n");
    for (int i = 0; i < count; i++)
    {
        printf("Package ID    : %d\n", packages[i].packageID);
        printf("Destination   : %s\n", packages[i].destination);
        printf("Days          : %d\n", packages[i].days);

        printf("Transport     : ");
        if (packages[i].transport == 1)
            printf("Train\n");
        else if (packages[i].transport == 2)
            printf("Aeroplane\n");
        else
            printf("Bus\n");

        printf("Accommodation : ");
        if (packages[i].accommodation == 1)
            printf("Budget Hotel\n");
        else if (packages[i].accommodation == 2)
            printf("Standard Hotel\n");
        else
            printf("Luxury Hotel\n");

        printf("Price/Person  : Rs. %.2f\n", packages[i].price);
        printf("----------------------------------\n");
    }
}

/*----------- Searches for a package using ID -----------*/
void searchPackage(Tour packages[], int count)
{
    int packageID, found = 0;
    if (count == 0)
    {
        printf("No packages available to search, first add package!\n");
        return;
    }

    printf("Enter package ID to search: ");
    packageID = getInt(1, 999999);

    for (int i = 0; i < count; i++)
    {
        if (packageID == packages[i].packageID)
        {
            found = 1;
            printf("\n--- Package Found ---\n");
            printf("Destination   : %s\n", packages[i].destination);
            printf("Days          : %d\n", packages[i].days);

            printf("Transport     : ");
            if (packages[i].transport == 1)
                printf("Train\n");
            else if (packages[i].transport == 2)
                printf("Aeroplane\n");
            else
                printf("Bus\n");

            printf("Accommodation : ");
            if (packages[i].accommodation == 1)
                printf("Budget Hotel\n");
            else if (packages[i].accommodation == 2)
                printf("Standard Hotel\n");
            else
                printf("Luxury Hotel\n");

            printf("Price/Person  : Rs. %.2f\n", packages[i].price);
            break;
        }
    }

    if (found == 0)
    {
        printf("Package not found!\n");
        return;
    }
}

/*----------- Updates the details of an existing package -----------*/
void updatePackage(Tour packages[], int count)
{
    int ID, found, i;
    char ch;

    if (count == 0)
    {
        printf("There is no package to update, First Add packages\n");
        return;
    }

    do
    {
        found = 0;
        printf("Enter the package ID you want to update: ");
        ID = getInt(1, 999999);

        for (i = 0; i < count; i++)
        {
            if (packages[i].packageID == ID)
            {
                found = 1;
                break;
            }
        }

        if (found == 1)
        {
            printf("\n=== Update Package ===\n");
            int duplicate;
            do
            {
                duplicate = 0;
                printf("New Package ID: ");
                packages[i].packageID = getInt(1, 999999);

                for (int j = 0; j < count; j++)
                {
                    if (j != i && packages[i].packageID == packages[j].packageID)
                    {
                        printf("ID already exists! Enter again.\n");
                        duplicate = 1;
                        break;
                    }
                }
            } while (duplicate);

            printf("New Destination: ");
            getText(packages[i].destination);

            printf("New Days: ");
            packages[i].days = getInt(1, 365);

            printf("New Mode of Transport:\n 1- Train\n 2- Aeroplane\n 3- Bus\nChoice: ");
            packages[i].transport = getInt(1, 3);

            printf("New Accommodation:\n 1- Budget Hotel\n 2- Standard Hotel\n 3- Luxury Hotel\nChoice: ");
            packages[i].accommodation = getInt(1, 3);

            printf("New Price per person (Rs.): ");
            packages[i].price = getPrice();

            saveData(packages, count);
            printf("Package updated and saved successfully!\n");
        }
        else
            printf("ID doesn't exist!\n");

        printf("Do you want to update more packages? (Y/N): ");
        scanf(" %c", &ch);
        clearInput();
    } while (ch == 'Y' || ch == 'y');
}

/*----------- Deletes a package and also adjusts the size of packages array -----------*/
void deletePackage(Tour packages[], int *count)
{
    int id, i, j, found = 0;
    char ch;

    if (*count == 0)
    {
        printf("No packages available to delete!\n");
        return;
    }

    do
    {
        found = 0;
        printf("Enter Package ID to delete: ");
        id = getInt(1, 999999);

        for (i = 0; i < *count; i++)
        {
            if (packages[i].packageID == id)
            {
                found = 1;
                for (j = i; j < *count - 1; j++)
                    packages[j] = packages[j + 1];

                (*count)--;
                saveData(packages, *count);
                printf("Package deleted successfully!\n");
                break;
            }
        }

        if (found == 0)
        {
            printf("Package ID not found! Do you want to try again? (Y/N): ");
            scanf(" %c", &ch);
            clearInput();
        }
    } while (found == 0 && (ch == 'Y' || ch == 'y'));
}

/*----------- Searches for a customer ID and returns index (-1 if not found) -----------*/
int findCustomer(customer customers[], int customerCount, int id)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].customerID == id)
            return i;
    }
    return -1;
}

/*----------- Register a customer and check for duplicate IDs -----------*/
void RegisterCustomer(customer customers[], int *customerCount)
{
    int id;
    char ch;
    do
    {
        if (*customerCount >= MAX)
        {
            printf("Customer limit reached!\n");
            break;
        }

        printf("\n===== REGISTER CUSTOMER =====\n");
        printf("Enter Customer ID: ");
        id = getInt(1, 999999);

        while (findCustomer(customers, *customerCount, id) != -1)
        {
            printf("This ID already exists! Enter another ID: ");
            id = getInt(1, 999999);
        }

        int i = *customerCount;
        customers[i].customerID = id;

        printf("Enter Customer Name: ");
        getText(customers[i].name);
        
         int j, valid;

            do
            {
                valid = 1;

                printf("Enter phone number: ");
                scanf("%19s", customers[i].phone);

                for (j = 0; customers[i].phone[j] != '\0'; j++)
                {
                    if (customers[i].phone[j] < '0' ||
                        customers[i].phone[j] > '9')
                    {
                        valid = 0;
                        break;
                    }
                }

                if (valid == 0)
                {
                    printf("Invalid! Enter digits only.\n");
                }

            } while (valid == 0);
        (*customerCount)++;
        saveCustomers(customers, *customerCount);
        printf("Customer registered and saved successfully!\n");

        printf("Do you want to register another customer (Y/N)? ");
        scanf(" %c", &ch);
        clearInput();
    } while (ch == 'y' || ch == 'Y');
}

/*----------- Displays a single customer's details by ID -----------*/
void viewCustomer(customer customers[], int customerCount)
{
    if (customerCount == 0)
    {
        printf("No customers registered!\n");
        return;
    }

    printf("Enter Customer ID to view: ");
    int idx = findCustomer(customers, customerCount, getInt(1, 999999));

    if (idx != -1)
    {
        printf("\n--- Customer Details ---\n");
        printf("Customer ID : %d\n", customers[idx].customerID);
        printf("Name        : %s\n", customers[idx].name);
        printf("Contact     : %s\n", customers[idx].phone);
    }
    else
        printf("Customer not found!\n");
}

/*----------- Deletes a customer and shifts remaining records left -----------*/
void deleteCustomer(customer customers[], int *customerCount)
{
    if (*customerCount == 0)
    {
        printf("No customers available to delete!\n");
        return;
    }

    printf("Enter Customer ID to delete: ");
    int idx = findCustomer(customers, *customerCount, getInt(1, 999999));

    if (idx == -1)
    {
        printf("Customer not found!\n");
        return;
    }

    for (int j = idx; j < *customerCount - 1; j++)
        customers[j] = customers[j + 1];

    (*customerCount)--;
    saveCustomers(customers, *customerCount);
    printf("Customer deleted successfully!\n");
}

/*----------- Calculates total cost, transport-based discount, and 9% tax -----------*/
float CalculateTotal(Tour *packages, int members, float *discount, float *tax)
{
    float subtotal = packages->price * members; // price per package is for one person

    if (packages->transport == 1)
        *discount = subtotal * 0.05;
    else if (packages->transport == 2)
        *discount = subtotal * 0.10;
    else
        *discount = subtotal * 0.03;

    float discountedTotal = subtotal - *discount;
    *tax = discountedTotal * 0.09; // 9% tax

    return discountedTotal + *tax;
}

/*----------- Finds packages according to customer budget -----------*/
void BudgetFriendly(Tour packages[], int count)
{
    if (count == 0)
    {
        printf("No packages available.\n");
        return;
    }

    int budget, members, found = 0;
    float total, discount, tax;

    printf("\nEnter Your Budget (Rs.): ");
    budget = getInt(1, 100000000);

    printf("Enter Number of Members: ");
    members = getInt(1, 100);

    printf("\n---- BUDGET FRIENDLY PACKAGES (Incl. Discount & 9%% Tax) ----\n");
    for (int i = 0; i < count; i++)
    {
        total = CalculateTotal(&packages[i], members, &discount, &tax);

        if (total <= budget)
        {
            printf("\nPackage ID    : %d", packages[i].packageID);
            printf("\nDestination   : %s (%d Days)", packages[i].destination, packages[i].days);
            printf("\nTransport     : %s", packages[i].transport == 1 ? "Train" : (packages[i].transport == 2 ? "Aeroplane" : "Bus"));
            printf("\nAccommodation : %s", packages[i].accommodation == 1 ? "Budget Hotel" : (packages[i].accommodation == 2 ? "Standard Hotel" : "Luxury Hotel"));
            printf("\nPrice/Person  : Rs. %.2f", packages[i].price);
            printf("\nDiscount      : -Rs. %.2f", discount);
            printf("\nTax (9%%)      : +Rs. %.2f", tax);
            printf("\nTotal for %d   : Rs. %.2f\n", members, total);
            found = 1;
        }
    }

    if (found == 0)
        printf("\nNo packages available within your budget!\n");
}

/*----------- Generates a formatted tour bill for a registered customer -----------*/
void generateBill(Tour packages[], int count, customer customers[], int customerCount)
{
    if (count == 0 || customerCount == 0)
    {
        printf("Register at least one customer and add one package first!\n");
        return;
    }

    printf("\n=== GENERATE TOUR BILL ===\n");
    printf("Enter Registered Customer ID: ");
    int cID = getInt(1, 999999);
    int cIdx = findCustomer(customers, customerCount, cID);
    if (cIdx == -1)
    {
        printf("Customer ID not found! Register customer first.\n");
        return;
    }

    printf("Enter Package ID to Book: ");
    int pID = getInt(1, 999999);
    int pIdx = -1;
    for (int i = 0; i < count; i++)
    {
        if (packages[i].packageID == pID)
        {
            pIdx = i;
            break;
        }
    }
    if (pIdx == -1)
    {
        printf("Package ID not found!\n");
        return;
    }

    printf("Enter Number of Travelers: ");
    int members = getInt(1, 100);

    float discount = 0, tax = 0;
    float baseTotal = packages[pIdx].price * members;
    float finalBill = CalculateTotal(&packages[pIdx], members, &discount, &tax);

    printf("\n========================================\n");
    printf("           TOUR PACKAGE BILL            \n");
    printf("========================================\n");
    printf("Customer Name : %s (ID: %d)\n", customers[cIdx].name, cID);
    printf("Contact       : %s\n", customers[cIdx].phone);
    printf("Destination   : %s (%d Days)\n", packages[pIdx].destination, packages[pIdx].days);
    printf("Transport     : %s\n", packages[pIdx].transport == 1 ? "Train" : (packages[pIdx].transport == 2 ? "Aeroplane" : "Bus"));
    printf("Accommodation : %s\n", packages[pIdx].accommodation == 1 ? "Budget Hotel" : (packages[pIdx].accommodation == 2 ? "Standard Hotel" : "Luxury Hotel"));
    printf("Travelers     : %d\n", members);
    printf("Base Price    : Rs. %.2f\n", baseTotal);
    printf("Discount      : -Rs. %.2f\n", discount);
    printf("Tax (9%%)      : +Rs. %.2f\n", tax);
    printf("----------------------------------------\n");
    printf("TOTAL BILL    : Rs. %.2f\n", finalBill);
    printf("========================================\n");
    printf("       THANK YOU FOR TRAVELLING!        \n");
}

/*----------- Password-protected Admin View for all customers and packages -----------*/
void adminViewAll(customer customers[], int customerCount, Tour packages[], int count)
{
    char pass[30];
    printf("Enter confidential admin password: ");
    scanf(" %29s", pass);
    clearInput();

    if (strcmp(pass, ADMIN_PASS) != 0)
    {
        printf("Access denied! Incorrect password.\n");
        return;
    }

    printf("\n--- ALL REGISTERED CUSTOMERS (%d) ---\n", customerCount);
    for (int i = 0; i < customerCount; i++)
        printf("ID: %d | Name: %s | Contact: %s\n", customers[i].customerID, customers[i].name, customers[i].phone);

    printf("\n--- ALL AVAILABLE PACKAGES (%d) ---\n", count);
    for (int i = 0; i < count; i++)
        printf("ID: %d | Destination: %s | Days: %d | Price: Rs. %.2f\n",
               packages[i].packageID, packages[i].destination, packages[i].days, packages[i].price);
}

/*----------- Save all package records into a text file -----------*/
int saveData(Tour packages[], int count)
{
    FILE *fp = fopen("packages.txt", "w");
    if (fp == NULL)
        return 0;

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d|%s|%d|%d|%d|%.2f\n",
                packages[i].packageID, packages[i].destination, packages[i].days,
                packages[i].transport, packages[i].accommodation, packages[i].price);
    }
    fclose(fp);
    return 1;
}

/*----------- Loads previously saved package records from text file -----------*/
void loadData(Tour packages[], int *count)
{
    FILE *fp = fopen("packages.txt", "r");
    if (fp == NULL)
        return;

    int i;
    for (i = 0; i < MAX; i++)
    {
        if (fscanf(fp, "%d|%49[^|]|%d|%d|%d|%f",
                   &packages[i].packageID, packages[i].destination, &packages[i].days,
                   &packages[i].transport, &packages[i].accommodation, &packages[i].price) != 6)
            break;
    }
    *count = i;
    fclose(fp);
}

/*----------- Saves customer records into a text file -----------*/
int saveCustomers(customer customers[], int customerCount)
{
    FILE *fp = fopen("customers.txt", "w");
    if (fp == NULL)
        return 0;

    for (int i = 0; i < customerCount; i++)
    {
        fprintf(fp, "%d|%s|%s\n", customers[i].customerID, customers[i].name, customers[i].phone);
    }
    fclose(fp);
    return 1;
}

/*----------- Loads previously saved customer records from a text file -----------*/
void loadCustomers(customer customers[], int *customerCount)
{
    FILE *fp = fopen("customers.txt", "r");
    if (fp == NULL)
        return;

    int i;
    for (i = 0; i < MAX; i++)
    {
        if (fscanf(fp, "%d|%49[^|]|%19[^\n]",
                   &customers[i].customerID, customers[i].name, customers[i].phone) != 3)
            break;
    }
    *customerCount = i;
    fclose(fp);
}
