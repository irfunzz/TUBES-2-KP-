// KETAPANG STORE
// simple online shop

// TEAM 8
// 1. Benedith Jeffierson Tanujaya - L0124044
// 2. Muhammad Irfan - L0124063
// 3. Rifqia Hani - L0124074

// ADD LIBRARIES
//=====================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

// DEFINE STRUCT MAX VALUE
//=====================================================================================
#define MAX_ITEMS 500 
#define MAX_CART 99 
#define MAX_ADRESS 500 
#define MAX_DELIVERY 10
#define MAX_USER 25 
#define MAX_REVIEWS 100
#define MAX_RETURN 100
#define MAX_LOG 1000

// COLOR
//=====================================================================================
#define NORM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"
#define BLACK "\033[30m"
#define ORANGE  "\x1B[38;5;214m"
#define PURPLE  "\x1B[38;5;57m"
#define PINK  "\x1B[38;5;213m"
#define BROWN  "\x1B[38;5;94m"
#define GREY  "\x1B[38;5;240m"
#define LIGHTGREY  "\x1B[38;5;250m"
#define LIGHTBLUE  "\x1B[38;5;39m"
#define LIGHTGREEN  "\x1B[38;5;77m"
#define LIGHTCYAN  "\x1B[38;5;51m"
#define LIGHTRED  "\x1B[38;5;196m"
#define LIGHTMAGENTA  "\x1B[38;5;201m"
//=====================================================================================

// SRTUCT DECLARATION
//=====================================================================================
typedef struct {
    long itemCode; //date item added     
    char category[50]; 
    char name[100];     
    int price;
    int stock;
    int weight;
    int bought;
    char tags[100];   
} warehouse;

typedef struct {
    long code;
    int amount;
    int cWeight; //cart weight (item weight * amount)
} cart;

typedef struct {
    char province[50];
    char city[50];
    int distance; //distance from 'solo' to ... (Km)
} maps;

typedef struct {
    char provider [50];
    int charge; 
    int speed;
} delivery;

typedef struct {
    char username[50];
    char password[50];
    char email[50];
    int userCode;
    int userAdressCode;
    int balance;
} id;

typedef struct {
    long itemDate;
    char itemName[50];
    long itemCode;
    int amount;
    int confirmReturn; //return accepted / not
    int checkReturn; //return seen / not
    int userReturn;
} returnItem;

typedef struct {
    int userCode;
    long dateBuyed;
    long dateArrived;
    char itemName[50];
    int amount;
    long itemCode;
} hist;
//=====================================================================================

//GLOBAL VARIABEL
//=====================================================================================
int warehouseCount = 0; 
int addressCount = 0;   
int cartCount = 0;
int deliveryCount = 0;
int userCount = 0;
int reviewCount = 0;
int returnCount = 0;
int logCount = 0;

char date [11];
int invoiceETASecond = 0; //item arrival date in second

int nowUsingCode = 0;
int isLoggedIn = 0;
int choosenItem;

int sw; //sw = switch
int repeat;
char choice;
//=====================================================================================

// STRUCT DECLARATION
//=====================================================================================
warehouse wItems[MAX_ITEMS]; //Warehouse Items
cart cItems[MAX_CART]; //Cart Items
maps address[MAX_ADRESS];
delivery courier[MAX_DELIVERY];
id user[MAX_USER];
returnItem rItems[MAX_RETURN]; // Return Items
hist logs[MAX_LOG];

// FUNCTION DECLARATION
//=====================================================================================
void mainMenu();

void clearScreen();
char* trimWhitespace(char* str);
void pressEnter();
void loading();
void calendar();

void printAllProductPaginated(int page);
void printPageUI(int page, int itemsPerPage);
void printLogo();

void readAll();
void readWarehouse(warehouse wItems[]);
void readAddress(maps address[]);
void readCart(cart cItems[]);
void readCourier(delivery courier[]);
void readUser (id user[]);
void readReturn(returnItem rItems[]);
void readLog(hist logs[]);

void updateUser ();
void updateWarehouse();
void updateReturn();
void updateLog(cart cItems[], warehouse wItems[], id user[]);
void clearCart();

void addUser (id *tempUser );
void addLog();

int login();

void regist();
int isUsernameUnique(const char *username);
void generateUniqueUsername(char *username);
int generateRandomNumber(int min, int max);
int isEmailUnique(const char *email);
void checkPassword(const char *password, int *strength);
int chooseAddress();

void buyerMenu();
void chooseBuyOrNot();
void printChoosenItem();
void addItemToCart(int choosenItem, int amount);
void reduceItemFromCart();
void requestReturn();
void topUpBalance();

void sortProductsMenu();
void sortProductsPrice(warehouse wItems[]);
void sortProductsSold(warehouse wItems[]);
void sortProductsArrival(warehouse wItems[]);

void searchMenu();
void advancedSearch(warehouse wItems[]);
void searchProductByCategory(warehouse wItems[]);
void searchProductByName(warehouse wItems[]);
void searchProductByTags(warehouse wItems[]);

void cartMenu(int mustPay);
void printCart(cart cItems[], warehouse wItems[], int mustPay);
void printInvoice(cart cItems[], warehouse wItems[], id user[], int chooseDelivery, int mustPay);
void saveInvoiceToFile(cart cItems[], warehouse wItems[], id user[], int chooseDelivery, int mustPay);

void chooseDelivery(int mustPay);
void printDeliveryList();
int countShipmentCost(cart cItems[], warehouse wItems[], delivery courier[], int count);
char* countETA(maps address[], delivery courier[], int count);

void sellerMenu();
void addStock();
void addNewItem();
void confirmReturn();
//=====================================================================================

// MAIN
//=====================================================================================
int main() {
    printLogo();
    calendar();
    readAll();
    mainMenu();
    return 0;
}

void mainMenu(){
    repeat = 1;

    do {
        clearScreen();
        printf("\n----------------------------- MAIN MENU -----------------------------");
        printf("\n1.\t LOGIN");
        printf("\n2.\t REGISTER");
        printf("\n3.\t SEARCH PRODUCTS");
        printf("\n%s99.\t EXIT", RED);
        printf("\n%sSelect Menu : %s", BLUE, LIGHTBLUE);
        if(scanf("%d", &sw) != 1){
            while (getchar() != '\n');
        };
        clearScreen();
        switch (sw) {
            case 1:
                if (login()) {
                    isLoggedIn = 1;
                    if (nowUsingCode != 0) {
                        buyerMenu();
                    } else if (nowUsingCode == 0) {
                        //only 1 seller (code == 0)
                        sellerMenu();
                    } else {
                        printf("%sUnknown user type!%s\n", RED, NORM);
                    }
                } else {
                    printf("%sLogin failed!%s\n", RED, NORM);
                }
                break;
            case 2:
                regist();
                break;
            case 3:
                searchMenu();
                break;
            case 99:
                printf("\nExiting...");
                exit(0);
                repeat = 0;
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (repeat == 1);
}
//=====================================================================================

// SYSTEM FUNCTION
//=====================================================================================
void clearScreen(){
    printf("\e[H\e[2J\e[3J"); //clear screen (cls)
    //header
    printf("\n%s=========================== KETAPANG STORE =========== %s ===", YELLOW, date);
    printf("\n=====================================================================%s", NORM);
}

char* trimWhitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void pressEnter(){
    printf ("\n\n%sPress Enter to Continue%s", BLUE, NORM);
    getchar(); //waiting enter
    getchar(); //to avoid bug (if there is buffer)
    clearScreen();
}

void loading(){
    printf ("\n\n");
    for (int i = 0; i < 3; i++){
        printf("\033[47m            \033[0m"); //print white loading bar
        sleep(1);
    }
};

void calendar (){
    //to count what time now (dd-mm-yy)
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    //populate the global date variable
    sprintf(date, "%02d-%02d-%d",
            localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
}
//=====================================================================================

// PRINTING
//=====================================================================================
void printAllProductPaginated(int page) {
    clearScreen();

    int itemsPerPage = 15;
    int start = (page - 1) * itemsPerPage;
    int end = start + itemsPerPage;

    if (start >= warehouseCount) {
        printf("\n\n%sEnd of page%s", RED, NORM);
        pressEnter();
        clearScreen();
        return;
    } else if (start < 0) {
        printf("\n\n%sInvalid page%s", RED, NORM);
        pressEnter();
        clearScreen();
        return;
    }

    printf("\n----------------------------- [Page 0%d] -----------------------------", page);
    printf("\n%-20s %-20s %-20s", "Product Name", "Category", "Price");

    for (int i = start; i < end && i < warehouseCount; i++) {
        printf("\n%-20s %-20s %-20d", 
                wItems[i].name, 
                wItems[i].category, 
                wItems[i].price);
    }

    printPageUI(page, itemsPerPage); 

    printf("\n\tnext page     : 'n'"); 
    printf("\n\tprevious page : 'p'"); 
    printf("\n\tselect item   : Enter item number (1 to %d)", end - start); 
    printf("\n\texit          : 'any key(other than n, p)'"); 

    char choice [10];
    scanf(" %s", choice); 
    clearScreen(); 

    if (strcmp(choice, "n") == 0 && end <= warehouseCount) { 
        printAllProductPaginated(page + 1); 
    } else if (strcmp(choice, "p") == 0 && start >= 0) { 
        printAllProductPaginated(page - 1); 
    } else { 
        int selectedItem = atoi(choice);
        if (selectedItem >= 1 && selectedItem <= (end - start)) {
            choosenItem = start + selectedItem - 1;
            printChoosenItem();
        } else {
            clearScreen(); 
        }
    } 
}

void printPageUI (int page, int itemsPerPage){
    //printing page navigation UI in the bottom of the page
    int lastPage = (warehouseCount + itemsPerPage - 1) / itemsPerPage;

    printf("\n---------------------------------------------------------------------");
    printf("\n<-- %-10s", "P");
    for (int i = page - 2; i <= page + 2; i++) {
        if (i <= 0){
            printf("%-10s", "");
        }
        else if (i > 0 && i < page){
            printf("%-10d", i);
        }
        else if (i == page){
            printf("\033[47m%s%d\033[0m%-9s", BLACK, i, ""); //black text in white background
        }
        else if (i > page && i <= lastPage){
            printf("%-10d", i);
        }
        else {
            printf("%-10s", "");
        }
    }

    printf("N -->");
    printf("\n---------------------------------------------------------------------");
}

void printLogo (){
    FILE *file;
    char ch;
    file = fopen("LOGO.txt", "r");
    if (file == NULL) {
        printf("Could not open file LOGO.txt\n");
    }
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    loading();
    clearScreen();
    fclose(file);
}

//READ TXT
//=====================================================================================
void readAll() {
    //read all txt files
    readWarehouse(wItems);
    readAddress(address);
    readCart(cItems);
    readCourier(courier);
    readUser(user);
    readLog(logs);
    readReturn(rItems);
}

void readWarehouse(warehouse wItems[]) {
    warehouseCount = 0;

    FILE *file = fopen("Warehouse.txt", "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (warehouseCount < MAX_ITEMS) {
        // Reset fields
        wItems[warehouseCount].itemCode = 0;
        strcpy(wItems[warehouseCount].category, "");
        strcpy(wItems[warehouseCount].name, "");
        wItems[warehouseCount].price = 0;
        wItems[warehouseCount].stock = 0;
        wItems[warehouseCount].weight = 0;
        wItems[warehouseCount].bought = 0;
        strcpy(wItems[warehouseCount].tags, "");

        int scanned = fscanf(file, "%ld | %49[^|] | %99[^|] | %d | %d | %d | %d | %99[^\n]", 
                             &wItems[warehouseCount].itemCode, 
                             wItems[warehouseCount].category, 
                             wItems[warehouseCount].name, 
                             &wItems[warehouseCount].price, 
                             &wItems[warehouseCount].stock, 
                             &wItems[warehouseCount].weight, 
                             &wItems[warehouseCount].bought, 
                             wItems[warehouseCount].tags);

        strcpy(wItems[warehouseCount].category, trimWhitespace(wItems[warehouseCount].category));
        strcpy(wItems[warehouseCount].name, trimWhitespace(wItems[warehouseCount].name));
        strcpy(wItems[warehouseCount].tags, trimWhitespace(wItems[warehouseCount].tags));

        if (scanned == 8) {
            warehouseCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }

    fclose(file);
}

void readAddress(maps address[]) {
    addressCount = 0;

    FILE *file = fopen("Adress.txt", "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (addressCount < MAX_ADRESS) {
        // Reset fields
        strcpy(address[addressCount].province, "");
        strcpy(address[addressCount].city, "");
        address[addressCount].distance = 0;

        int scanned = fscanf(file, "%49[^|] | %49[^|] | %d", 
                             address[addressCount].province, 
                             address[addressCount].city, 
                             &address[addressCount].distance);

        strcpy(address[addressCount].province, trimWhitespace(address[addressCount].province));
        strcpy(address[addressCount].city, trimWhitespace(address[addressCount].city));

        if (scanned == 3) {
            addressCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }
    fclose(file);
}

void readCart(cart cItems[]) {
    cartCount = 0;

    char filename[50];
    snprintf(filename, sizeof(filename), "Cart_%d.txt", user[nowUsingCode].userCode); // Adjust if user ID is dynamic

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    cartCount = 0;

    while (cartCount < MAX_CART) {
        // Reset fields
        cItems[cartCount].code = 0;
        cItems[cartCount].amount = 0;
        cItems[cartCount].cWeight = 0;

        int scanned = fscanf(file, "%ld | %d", 
                             &cItems[cartCount].code, 
                             &cItems[cartCount].amount);

        if (scanned == 2) {
            cartCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }

    fclose(file);
}

void readCourier(delivery courier[]) {
    deliveryCount = 0;
    
    FILE *file = fopen("Delivery.txt", "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (deliveryCount < MAX_DELIVERY) {
        // Reset fields
        strcpy(courier[deliveryCount].provider, "");
        courier[deliveryCount].charge = 0;
        courier[deliveryCount].speed = 0;

        int scanned = fscanf(file, " %49[^|] | %d | %d ", 
                             courier[deliveryCount].provider, 
                             &courier[deliveryCount].charge, 
                             &courier[deliveryCount].speed);

        if (scanned == 3) {
            deliveryCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }

    fclose(file);
}

void readUser(id user[]) {
    userCount = 0;
    
    FILE *file = fopen("userid.txt", "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (userCount < MAX_USER) {
        // Reset fields
        strcpy(user[userCount].username, "");
        strcpy(user[userCount].password, "");
        strcpy(user[userCount].email, "");
        user[userCount].userCode = 0;
        user[userCount].userAdressCode = 0;
        user[userCount].balance = 0;

        int scanned = fscanf(file, "%49[^|] | %49[^|] | %49[^|] | %d | %d | %d\n", 
                            user[userCount].username,
                            user[userCount].password,
                            user[userCount].email,
                            &user[userCount].userCode,
                            &user[userCount].userAdressCode,
                            &user[userCount].balance);

        strcpy(user[userCount].username, trimWhitespace(user[userCount].username));
        strcpy(user[userCount].email, trimWhitespace(user[userCount].email));
        strcpy(user[userCount].password, trimWhitespace(user[userCount].password));

        if (scanned == 6) {
            userCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }

    fclose(file);
}

void readReturn(returnItem rItems[]) {
    returnCount = 0;

    FILE *file = fopen("return.txt", "r");
    if (file == NULL) {
        printf("Error opening return file.\n");
        return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (returnCount < MAX_RETURN) {
        // Reset fields
        rItems[returnCount].itemCode = 0;
        strcpy(rItems[returnCount].itemName, "");
        rItems[returnCount].amount = 0;
        rItems[returnCount].confirmReturn = 0;
        rItems[returnCount].checkReturn = 0;
        rItems[returnCount].userReturn = 0;

        int scanned = fscanf(file, "%ld | %49[^|] | %ld | %d | %d | %d | %d\n", 
                                    &rItems[returnCount].itemDate,
                                    rItems[returnCount].itemName,
                                    &rItems[returnCount].itemCode, 
                                    &rItems[returnCount].amount, 
                                    &rItems[returnCount].confirmReturn, 
                                    &rItems[returnCount].checkReturn,
                                    &rItems[returnCount].userReturn);

        strcpy(rItems[returnCount].itemName, trimWhitespace(rItems[returnCount].itemName));

        if (scanned == 7) {
            returnCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }
    fclose(file);
}

void readLog(hist logs[]) {
    logCount = 0;

    FILE *file = fopen("log.txt", "r");
    if (file == NULL) {
        return;
    }
    fscanf(file, "%*[^\n]\n"); // Skip header
    while (logCount < MAX_LOG) {
        // Reset fields
        strcpy(logs[logCount].itemName, "");
        logs[logCount].userCode = 0;
        logs[logCount].dateBuyed = 0;
        logs[logCount].dateArrived = 0;
        logs[logCount].itemCode = 0;
        logs[logCount].amount = 0;

        int scanned = fscanf(file, "\n%d | %ld | %ld | %49[^|] | %d | %ld", 
                &logs[logCount].userCode, 
                &logs[logCount].dateBuyed,
                &logs[logCount].dateArrived,
                logs[logCount].itemName, 
                &logs[logCount].amount, 
                &logs[logCount].itemCode);

        strcpy(logs[logCount].itemName, trimWhitespace(logs[logCount].itemName));        

        if (scanned == 6) {
            logCount++;
        } else if (scanned == EOF) {
            break;
        } else {
            fscanf(file, "%*[^\n]\n");  // Skip the problematic line
        }
    }
}
//=====================================================================================

//UPDATE TXT
//=====================================================================================
void updateUser () {
    FILE *userid = fopen("userid.txt", "w");
    if (userid == NULL) {
        printf("Could not open user file for writing.\n");
        return;
    }
    fprintf(userid, "%-20s | %-20s | %-20s | %-10s | %-10s | %-10s", 
            "Username", "Pass", "Email", "Code", "Adress", "Balance"); //print header
    for (int i = 0; i < userCount; i++) {
        fprintf(userid, "\n%-20s | %-20s | %-20s | %-10d | %-10d | %-10d",
                user[i].username,
                user[i].password,
                user[i].email,
                user[i].userCode,
                user[i].userAdressCode,
                user[i].balance);
    }
    fclose(userid);
    readUser(user);
}

void updateWarehouse() {
    FILE *file = fopen("Warehouse.txt", "w");
    if (file == NULL) {
        printf("Could not open warehouse file for writing.\n");
        return;
    }
    fprintf(file, "%-20s | %-25s | %-25s | %-10s | %-10s | %-10s | %-10s | %-20s", 
            "Time", "Category", "Name", "Price", "Stock", "Weight", "Sold", "Tags"); //print header
    for (int i = 0; i < warehouseCount; i++) {
        fprintf(file, "\n%-20ld | %-25s | %-25s | %-10d | %-10d | %-10d | %-10d | %-20s", 
                wItems[i].itemCode, 
                wItems[i].category, 
                wItems[i].name, 
                wItems[i].price, 
                wItems[i].stock, 
                wItems[i].weight, 
                wItems[i].bought, 
                wItems[i].tags);
    }
    fclose(file);
    readWarehouse(wItems);
}

void updateReturn() {
    FILE *file = fopen("return.txt", "w");
    if (file == NULL) {
        printf("Could not open return file for writing.\n");
        return;
    }
    fprintf(file, "%-20s | %-20s | %-20s | %-10s | %-10s | %-10s | %-10s", 
            "Time", "Name", "Code", "Amount", "Confirm", "Check", "User"); //print header
    for (int i = 0; i < returnCount; i++) {
        fprintf(file, "\n%-20ld | %-20s | %-20ld | %-10d | %-10d | %-10d | %-10d", 
                rItems[i].itemDate, 
                rItems[i].itemName, 
                rItems[i].itemCode, 
                rItems[i].amount, 
                rItems[i].confirmReturn, 
                rItems[i].checkReturn,
                rItems[i].userReturn);
    }
    fclose(file);
    readReturn(rItems);
}

void updateLog(cart cItems[], warehouse wItems[], id user[]) {
    FILE *file = fopen("log.txt", "a");
    if (file == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fscanf(file, "%*[^\n]\n"); //print header
    for (int i = 0; i < cartCount; i++) {
        logs[logCount].userCode = user[nowUsingCode].userCode;
        logs[logCount].dateBuyed = time(NULL);
        logs[logCount].dateArrived = invoiceETASecond;
        logs[logCount].amount = cItems[i].amount;
        logs[logCount].itemCode = cItems[i].code;
        for (int j = 0; j < warehouseCount; j++) {
            if (wItems[j].itemCode == cItems[i].code) {
                strcpy(logs[logCount].itemName, wItems[j].name);
                break;
            }
        }

        fprintf(file, "\n%-10d | %-10ld | %-10ld | %-10s | %-10d | %-10ld", 
                logs[logCount].userCode, 
                logs[logCount].dateBuyed, 
                logs[logCount].dateArrived,
                logs[logCount].itemName, 
                logs[logCount].amount, 
                logs[logCount].itemCode);

        logCount++;
    }


    fclose(file);
}

void clearCart() {
    //clearing cart.txt by overwriting
    char filename[50]; 
    snprintf(filename, sizeof(filename), "Cart_%d.txt", user[nowUsingCode].userCode);

    FILE *file = fopen(filename, "w"); 
    if (file == NULL) { 
        printf("Error opening cart file for clearing. Please try again.\n"); 
        return; 
    } 

    fprintf(file, " CODE | AMOUNT\n"); //print header
    fclose(file);
    cartCount = 0;
}
//=====================================================================================

// ADD TXT
//=====================================================================================
void addUser(id *tempUser){
    FILE *userFile = fopen("userid.txt", "a+");
    if (userFile == NULL) {
        printf("Not found\n");
        return;
    }

    fprintf(userFile, "\n%-20s | %-20s | %-20s | %-10d | %-10d | %-10d", 
            tempUser->username, 
            tempUser->password, 
            tempUser->email, 
            userCount, 
            tempUser->userAdressCode,
            0);
    fclose(userFile);

    printf("%sRegistration was successful!\n%s", GREEN, NORM);
    readAll();
}

void addLog() {
    FILE *file = fopen("log.txt", "w");
    if (file == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(file, "%-20s | %-20s | %-20s | %-10s | %-10s | %-10s ", "USER CODE", "DATE", "ARRIVED", "ITEM", "AMOUNT", "ITEM CODE");
    for (int i = 0; i < logCount; i++) {
        fprintf(file, "\n%-20d | %-20ld | %-20ld | %-10s | %-10d | %-10ld", 
        logs[i].userCode, 
        logs[i].dateBuyed, 
        logs[i].dateArrived, 
        logs[i].itemName, 
        logs[i].amount, 
        logs[i].itemCode);
    }
    fclose(file);
    readLog(logs);

}
//=====================================================================================

// LOGIN
//=====================================================================================
int login() {
    char email[50];
    char password[50];
    clearScreen();

    printf("\n%s------------------------------- LOGIN -------------------------------%s\n", GREEN, NORM);
    printf("Enter Email    : ");
    scanf(" %s", email);
    printf("Enter Password : ");
    scanf(" %s", password);

    int found = 0;

    for (int i = 0; i <= userCount; i++ ){
        //compare input with data (userid.txt)
        if (strcmp(user[i].email, email) == 0 && strcmp(user[i].password, password) == 0) {
            nowUsingCode = i;
            printf("%s--------------------------- LOGIN SUCCESS ---------------------------%s", GREEN, NORM);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("%sEmail and Password are incorrect%s\n" RED, NORM);
    }

    pressEnter();

    return found;
}
//=====================================================================================

// REGIST
//=====================================================================================
void regist() {
    id tempUser;
    char confirmPassword[50];
    char suggestedUsername[50];
    int strength;
    int city;

    printf("\n%s------------------------------ REGISTER -----------------------------%s", GREEN, NORM);

    //username
    do {
        printf("\nUsername\t\t: ");
        scanf(" %[^\n]", tempUser.username);
        getchar();

        if (!isUsernameUnique(tempUser.username)) {
            printf("%sUsername is already taken%s", RED, NORM);
            strcpy(suggestedUsername, tempUser.username);
            generateUniqueUsername(suggestedUsername);
            printf("\nSuggested username\t: %s", suggestedUsername);
            printf("%s\nPress Y to continue, N to change username: %s", BLUE, NORM);
            scanf (" %c", &choice);
            if (choice == 'n'){
                printf("lawak");
                pressEnter();
                continue;
            } else if (choice == 'Y' || choice == 'y') {
                strcpy(tempUser.username, suggestedUsername);
                break;
            }
        }
    } while (!isUsernameUnique(tempUser.username));

    //email
    do {
        printf("Email\t\t\t: ");
        scanf(" %s", tempUser.email);

        if (!isEmailUnique(tempUser.email)) {
            printf("%sEmail is already registered%s\n", RED, NORM);
            printf("%s\nPress Y to login, N to enter new email: %s", BLUE, NORM);
            scanf (" %c", &choice);
            if (choice == 'y' || choice == 'Y'){
                login();
                break;
            } else if (choice == 'n' || choice == 'N') {
                continue;
            }
        }
    } while (!isEmailUnique(tempUser.email));

    //password
    do {
        printf("Password\t\t: ");
        scanf(" %s", tempUser.password);
        printf("Confirm Password\t: ");
        scanf(" %s", confirmPassword);

        if (strcmp(tempUser.password, confirmPassword) != 0) {
            printf("%sPassword does not match\n%s", RED, NORM);
            continue;
        }

        checkPassword(tempUser.password, &strength);

        if (strength == 2) {
            printf("%sPassword is strong!\n%s", GREEN, NORM);
        } else if (strength == 1) {
            printf("%sPassword is mid!\n%s", YELLOW, NORM);
        } else if (strength == 0) {
            printf("%sPassword is too weak!\n%s", RED, NORM);
        }

        if (strength < 2) {
            printf("%sDo you wish to replace the password? %s(Y/N)%s: ", NORM, BLUE, NORM);
            scanf(" %c", &choice);
        } else {
            choice = 'N'; 
        }

    } while (toupper(choice) == 'Y');

    //address
    readAddress(address);
    tempUser.userAdressCode = chooseAddress();

    addUser(&tempUser);
}

int isUsernameUnique(const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(user[i].username, username) == 0) {
            return 0; 
        }
    }
    return 1; 
}

void generateUniqueUsername(char *username) {
    char newUsername[50];
    do {
        strcpy(newUsername, username);
        int randomDigits = generateRandomNumber(1, 999);
        char digits[4];
        sprintf(digits, "%d", randomDigits);
        strcat(newUsername, digits);
    } while (!isUsernameUnique(newUsername));
    strcpy(username, newUsername);
}

int generateRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

int isEmailUnique(const char *email) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(user[i].email, email) == 0) {
            return 0; 
        }
    }
    return 1;
}

void checkPassword(const char *password, int *strength) {
    int length = strlen(password);
    int upperchar = 0; 
    int lowerchar = 0; 
    int specialchar = 0; 
    int digitchar = 0;
    const char specialcharacter[] = "!@#$%%^&*()-_=+[{]}|;:'\"\\,<.>/?";

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            upperchar++;
        } else if (islower(password[i])) {
            lowerchar++;
        } else if (isdigit(password[i])) {
            digitchar++;
        } else if (strchr(specialcharacter, password[i])) {
            specialchar++;
        }
    }

    if (length < 6) {
        *strength = 0;
    } else if (length >= 6 && upperchar >= 1 && lowerchar >= 1) {
        if (digitchar >= 1 && specialchar >= 1) {
            *strength = 2;
        } else {
            *strength = 1;
        }
    } else {
        *strength = 0;
    }
}

int chooseAddress() {
    char inputProvince[50];
    int city;
    int counter = 0;
    int start = -1;

    do {
        while (getchar() != '\n');
        
        printf("Province: ");
        scanf("%[^\n]", inputProvince);

        for (int i = 0; i < addressCount; i++){
            if (strcmp(address[i].province, inputProvince) == 0) {
                printf("\n%d\t%s", i, address[i].city);
                counter++;
                if (start == -1){
                    start = i;
                }
            }
        }

        if (counter == 0) {
            printf("\n%sProvince not found. Please try again.%s", RED, NORM);
            continue;
        }

        printf("\nCity (%d - %d): ", start, (start + counter - 1));
        scanf("%d", &city);

        if (city < (start) || city >= (start + counter)) {
            printf("\n%sInvalid city index. Please try again.%s", RED, NORM);
            continue;
        }

        printf("\nAddress: %s, %s", address[city].province, address[city].city);
        printf("\nProceed (Y/N): ");
        scanf(" %c", &choice);

    } while (toupper(choice) != 'Y');

    return city;
}
//=====================================================================================

// BUYER
//=====================================================================================
void buyerMenu() {
    repeat = 1;
    int mustPay = 0;

    do {
        clearScreen();
        printf("\n----------------------------- MAIN MENU -----------------------------");
        printf("\n1.\t SHOW ALL ITEMS");
        printf("\n2.\t SEARCH");
        printf("\n3.\t CHECK CART");
        printf("\n4.\t REDUCE ITEM FROM CART");
        printf("\n5.\t REQUEST RETURN");
        printf("\n6.\t TOP UP");
        printf("\n99.\t LOG OUT");
        printf("\n%sSelect Menu : %s", BLUE, NORM);
        if(scanf("%d", &sw) != 1) {
            while(getchar() != '\n');
        }
        clearScreen();
        switch (sw) {
            case 1:
                sortProductsMenu();
                break;
            case 2:
                searchMenu();
                break;
            case 3:
                readCart(cItems);
                cartMenu(mustPay);
                break;
            case 4:
                reduceItemFromCart();
                break;
            case 5:
                requestReturn();
                break;
            case 6:
                topUpBalance();
                break;
            case 99:
                mainMenu();
                repeat = 0;
                break;
            default:
                printf("Invalid Menu");
                break;
        }
    } while (repeat == 1);
}

void chooseBuyOrNot (){
    int tempChoose;

    printf("\nEnter 'N' to Return");
    printf("\nEnter Item Code: ");

    getchar();
    if (scanf ("%d", &tempChoose) != 1) {
        while(getchar() != '\n');
        searchMenu(choosenItem);
    }
    else {
        choosenItem = tempChoose;
    }

    getchar();
    printChoosenItem ();
}

void printChoosenItem() {
    if (choosenItem < 0 || choosenItem >= warehouseCount) {
        printf("Invalid item selection.\n");
        buyerMenu();
    }

    warehouse chosenItemDetails = wItems[choosenItem];

    clearScreen();
    printf("\n---------------------------- ITEM DETAIL ----------------------------");
    printf("\n---------------------------------------------------------------------");
    printf("\nName: %-20s", chosenItemDetails.name);
    printf("\nPrice: %-20d", chosenItemDetails.price);
    printf("\nSold: %-20d", chosenItemDetails.bought);
    printf("\n---------------------------------------------------------------------");

    if (isLoggedIn == 1){
        printf("\nDo you want to buy this item? (Y/N): ");
        scanf(" %c", &choice); 

        if (choice == 'Y' || choice == 'y') {
            int amount;
            getchar();
            printf("Enter the quantity to add to cart: ");
            scanf("%d", &amount);
            addItemToCart(choosenItem, amount);
            buyerMenu();
        } else {
            buyerMenu();
        }
    }
    else {
        printf("\nPlease login to buy this item");
        pressEnter();
        mainMenu();
    }

}

void addItemToCart(int choosenItem, int amount) {
    if (choosenItem < 0 || choosenItem >= warehouseCount) {
        printf("Invalid item selection.\n");
        return;
    }

    if (amount <= 0) {
        printf("Invalid amount. Please enter a positive number.\n");
        return;
    }

    cItems[cartCount].code = wItems[choosenItem].itemCode;
    cItems[cartCount].amount = amount;
    cItems[cartCount].cWeight = wItems[choosenItem].weight;
    cartCount++; 

    FILE *file;
    char filename[50];
    snprintf(filename, sizeof(filename), "Cart_%d.txt", user[nowUsingCode].userCode); // Adjust filename based on user ID

    // Check if the file already has the header
    file = fopen(filename, "r");
    int hasHeader = 0;
    if (file != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, "CODE") && strstr(line, "AMOUNT")) {
                hasHeader = 1;
                break;
            }
        }
        fclose(file);
    }

    file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Error opening cart file.\n");
        return;
    }

    // If the header is not present, add it
    if (!hasHeader) {
        fprintf(file, "CODE | AMOUNT\n");
    }

    fprintf(file, "%ld | %d\n", cItems[cartCount - 1].code, cItems[cartCount - 1].amount); // Write the new item to the file
    fclose(file);

    printf("Item added to cart successfully!\n");

    pressEnter();
}

void reduceItemFromCart() {
    char wichItems[50];
    int amount;
    int found = 0;
    readCart(cItems);

    printf("\n--------------------------- REDUCE ITEM ---------------------------\n");
    for(int i = 0; i < cartCount; i++) {
        for(int j = 0; j < warehouseCount; j++) {
            if (cItems[i].code == wItems[j].itemCode) {
                printf("Item   : %s\n", wItems[j].name);
                printf("Amount : %d\n", cItems[i].amount);
                printf("-----------------------------------------------\n");
            }
        }
    }

    printf("Enter the item name: ");
    scanf(" %[^\n]", wichItems);

    for(int i = 0; i < cartCount; i++) {
        for(int j = 0; j < warehouseCount; j++) {
            if (strcmp(wichItems, wItems[j].name) == 0) {
                if (wItems[j].itemCode == cItems[i].code) {
                    printf("Enter the amount to reduce: ");
                    scanf(" %d", &amount);

                    if (amount <= 0) {
                        printf("Invalid amount. Please enter a positive number. \n");
                        return;
                    }

                    if (cItems[i].amount >= amount) {
                        cItems[i].amount -= amount;
                        found = 1;
                        break;
                    } else {
                        printf("You cannot reduce more than you have in the cart. \n");
                        pressEnter();
                        return;
                    }
                }
            }
        }
    }


    if (!found) {
        printf("Item not found in cart.\n");
        pressEnter();
        return;
    }

    FILE *file;
    char filename[50];
    snprintf(filename, sizeof(filename), "Cart_%d.txt", user[nowUsingCode].userCode); // Adjust filename based on user ID

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening cart file.\n");
        return;
    }

    fprintf(file, " CODE | AMOUNT\n"); //print header
    for (int i = 0; i < cartCount; i++) {
        fprintf(file, "%ld | %d\n", cItems[i].code, cItems[i].amount); // Write the new item to the file
    }
    fclose(file);

    printf("Item reduced from cart successfully!\n");

    pressEnter();
}

void requestReturn() {
    getchar();
    readLog(logs);
    char itemName[50];
    long itemCode;
    long date;

    printf("\n--------------------------- REQUEST RETURN ---------------------------\n");
    printf("\nYour purchase history:\n");
    for (int i = 0; i < logCount; i++) {
        if ((logs[i].userCode == user[nowUsingCode].userCode && logs[i].amount > 0)) {
            printf("Item     : %s\n", logs[i].itemName);
            printf("Amount   : %d\n", logs[i].amount);
            for (int j = 0;j < logCount; j++) {
                if ((logs[i].dateArrived == logs[j].dateArrived) && (logs[i].userCode == logs[j].userCode) && (logs[i].itemCode == logs[j].itemCode) && (j != i)) {
                    printf("Returned : %d\n", abs(logs[j].amount));
                    printf("Bought   : %d\n", logs[i].amount + logs[j].amount);
                }
            }
            printf("-----------------------------------------------\n");
        }
    }

    printf("\n%sEnter the name of the item you want to return", BLUE);
    printf("or %s'press N to return'%s):", ORANGE, NORM);
    scanf(" %[^\n]", itemName);

    if (toupper(itemName[0]) == 'N' && itemName[1] == '\0') {
        return; //back to buyer menu
    }

    // Check if the item was received within the last 7 days after ETA
    time_t currentTime = time(NULL);
    int found = 0;

    for (int i = 0; i < logCount; i++) {
        if (logs[i].userCode == user[nowUsingCode].userCode && strcmp(logs[i].itemName, itemName) == 0) {
            long eta = logs[i].dateArrived + 7 * 24 * 60 * 60; // ETA + 7 days in seconds
            double seconds = difftime(currentTime, eta);
            itemCode = logs[i].itemCode;
            date = logs[i].dateBuyed;
            if (seconds <= 0) {
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("%sItem cannot be returned, the same item has already been returned", RED);
        printf("\nItem not found or return window expired.%s\n", NORM);
        pressEnter();
        return;
    }

    int amount;
    printf("Enter the amount to return: ");
    scanf(" %d", &amount);
    
    // Check if user return the right amount
    for(int i = 0; i < logCount; i++) {
        if(logs[i].userCode == user[nowUsingCode].userCode && logs[i].itemCode == itemCode) {
            if (logs[i].amount >= amount) {
                logs[i].amount -= amount;
                break;
            } else {
                printf("You cannot return more than you bought.\n");
                pressEnter();
                return;
            }
        } 
    }

    //check if user can return
    int canReturn = 1;
    
    FILE *file = fopen("return.txt", "a");
    if (file == NULL) {
        printf("Error opening return file.\n");
        return;
        }

    //looping through all returned item
    for (int i = 0; i < returnCount; i++){
        if(rItems[i].itemCode == itemCode && rItems[i].itemDate == date) {
            //compare log with amount that will be returned
            for(int j = 0; j < logCount; j++) {
                if(nowUsingCode == logs[j].userCode) {
                    canReturn = 0;
                    printf("Cannot Return");
                    break;
                }
            }
        }
    }

    if (canReturn == 1){
    fprintf(file, "\n%-20ld | %-20s | %-20ld | %-10d | %-10d | %-10d | %-10d", 
            date, itemName, itemCode, amount, 0, 0, nowUsingCode);
    printf("Return request submitted successfully.\n");
    }

    fclose(file);
    readReturn(rItems);
    updateReturn();

    pressEnter();
}

void topUpBalance() {
    int amount;
    printf("\nEnter the amount to top up: ");
    scanf("%d", &amount);

    if (amount <= 0) {
        printf("Invalid amount. Please enter a positive number.\n");
        return;
    }

    user[nowUsingCode].balance += amount;

    updateUser();

    printf("Top up successful! Your new balance is: %d\n", user[nowUsingCode].balance);
    pressEnter();
}
//=====================================================================================

// SORTING
//=====================================================================================
void sortProductsMenu() {
    int page = 1;

    clearScreen();
    repeat = 0;
    do {
        printf("\n----------------------------- SORT MENU -----------------------------");
        printf("\n1.\t SORT BY PRICE");
        printf("\n2.\t SORT BY SOLD");
        printf("\n3.\t SORT BY ARRIVAL");
        printf("\n99.\t BACK");
        printf("\n%sSelect Menu : %s", BLUE, NORM);
        scanf("%d", &sw);
        clearScreen();
        switch (sw) {
            case 1:
                sortProductsPrice(wItems);
                printAllProductPaginated(page);
                break;
            case 2:
                sortProductsSold(wItems);
                printAllProductPaginated(page);
                break;
            case 3:
                sortProductsArrival(wItems);
                printAllProductPaginated(page);
                break;
            case 99:
                break;
            default:
                printf("\nInvalid Menu\n");
                repeat = 1;
        }
        buyerMenu();
    } while (repeat = 1);
}

void sortProductsPrice(warehouse wItems[]) {

    printf("\n--------------------------- SORT BY PRICE ---------------------------");
    printf("\nSORT FROM");
    printf("\n1.\t Highest Price");
    printf("\n2.\t Lowest Price");
    printf("\n99.\t BACK");
    printf("\n%sSelect Menu : %s", BLUE, NORM);

    scanf("%d", &sw);
    clearScreen();
    switch (sw) {
        case 1:
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].price < wItems[j].price) {
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].price > wItems[j].price) {
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 99:
            sortProductsMenu();
            break;
        default:
            printf("Invalid Menu");
            pressEnter();
            break;
    }

}

void sortProductsSold(warehouse wItems[]) {
;
    printf("\n-------------------------- SORT BY SELLING --------------------------");
    printf("\nSORT FROM");
    printf("\n1.\t Highest Selling");
    printf("\n2.\t Lowest Selling");
    printf("\n99.\t BACK");
    printf("\n%sSelect Menu : %s", BLUE, NORM);

    scanf("%d", &sw);

    switch (sw) {
        case 1:
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].bought < wItems[j].bought) {
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].bought > wItems[j].bought) {
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 99:
            sortProductsMenu();
            break;
        default:
            printf("Invalid Menu");
            pressEnter();
            break;
    }

}

void sortProductsArrival(warehouse wItems[]) {

    printf("\n-------------------------- SORT BY ARRIVAL --------------------------");
    printf("\nSORT FROM");
    printf("\n1.\t Latest Product");
    printf("\n2.\t Oldest Product");
    printf("\n99.\t BACK");
    printf("\n%sSelect Menu : %s", BLUE, NORM);

    scanf("%d", &sw);

    switch (sw) {
        case 1:
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].itemCode > wItems[j].itemCode) { 
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 2: 
            for (int i = 0; i < warehouseCount - 1; i++) {
                for (int j = i + 1; j < warehouseCount; j++) {
                    if (wItems[i].itemCode < wItems[j].itemCode) { 
                        warehouse temp = wItems[i];
                        wItems[i] = wItems[j];
                        wItems[j] = temp;
                    }
                }
            }
            break;
        case 99:
            sortProductsMenu();
            return;
        default:
            printf("Invalid Menu");
            pressEnter();
            return;
    }
}
//=====================================================================================

// SEARCHING
//=====================================================================================
void searchMenu() {
    clearScreen();

    do {
        printf("\n%s---------------------------- SEARCH MENU ----------------------------%s", GREEN, NORM);
        printf("\n%s1.\t SEARCH BY NAME%s", LIGHTGREEN, NORM);
        printf("\n%s2.\t SEARCH BY CATEGORY%s", LIGHTGREEN, NORM);
        printf("\n%s3.\t SEARCH BY TAGS%s", LIGHTGREEN, NORM);
        printf("\n%s4.\t ADVANCED SEARCH%s", LIGHTGREEN, NORM);
        printf("\n%s99.\t BACK%s", RED, NORM);
        printf("\n%sSelect Menu:%s ", BLUE, LIGHTBLUE);
        scanf("%d", &sw);
        clearScreen();
        switch (sw) {
            case 1:
                searchProductByName(wItems);
                chooseBuyOrNot();
                repeat = 0;
                break;
            case 2:
                searchProductByCategory(wItems);
                chooseBuyOrNot();
                repeat = 0;
                break;
            case 3:
                searchProductByTags(wItems);
                chooseBuyOrNot();
                repeat = 0;
                break;
            case 4:
                advancedSearch(wItems);
                chooseBuyOrNot();
                repeat = 0;
                break;
            case 99:
                if (isLoggedIn == 1){
                    buyerMenu();
                }
                else{
                    mainMenu();
                }
            default:
                repeat = 1;
                break;
        }
    } while (repeat == 1);
}

void advancedSearch(warehouse wItems[]) {
    char query[100];
    char category[50];
    int minPrice, maxPrice;

    printf("\n---------------------------- ADVANCED SEARCH ----------------------------\n");

    printf("Enter product name (or enter '1' to skip): ");
    fgets(query, sizeof(query), stdin);
    if (strcmp(query, "1\n") == 0) {
        query[0] = '\0'; // Skip name filter
    } else {
        query[strcspn(query, "\n")] = '\0'; // Remove newline character
    }
    getchar(); // Clear input buffer

    printf("Enter category (or enter '1' to skip): ");
    fgets(category, sizeof(category), stdin);
    if (strcmp(category, "1\n") == 0) {
        category[0] = '\0'; // Skip category filter
    } else {
        category[strcspn(category, "\n")] = '\0'; // Remove newline character
    }
    getchar(); // Clear input buffer

    printf("Enter minimum price (or -1 to skip): ");
    scanf("%d", &minPrice);

    printf("Enter maximum price (or -1 to skip): ");
    scanf("%d", &maxPrice);

    printf("\nSearch Results:\n");
    for (int i = 0; i < warehouseCount; i++) {
        int match = 1;

        if (strlen(query) > 0 && strstr(wItems[i].name, query) == NULL) {
            match = 0;
        }

        if (strlen(category) > 0 && strcmp(wItems[i].category, category) != 0) {
            match = 0;
        }

        if (minPrice != -1 && wItems[i].price < minPrice) {
            match = 0;
        }

        if (maxPrice != -1 && wItems[i].price > maxPrice) {
            match = 0;
        }

        if (match) {
            printf("\n%d.\t %-10s %-20s %-40d", i, wItems[i].tags, wItems[i].name, wItems[i].price);
        }
    }
}

void searchProductByCategory(warehouse wItems[]) {
    
    char query[100];

    printf("\nEnter search query: ");
    scanf(" %[^\n]", query);

        printf("Search Results:\n");
        for (int i = 0; i < warehouseCount; i++) {
            if (strstr(wItems[i].name, query) != NULL || strstr(wItems[i].category, query) != NULL) {
                printf("\n%d.\t %-20s %-20s %-20d", i, wItems[i].tags, wItems[i].name, wItems[i].price);
            }
        }
}

void searchProductByName(warehouse wItems[]) {
    char query[100];

    printf("\nEnter product name to search: ");
    scanf(" %[^\n]", query);

    printf("Search Results:");
    for (int i = 0; i < warehouseCount; i++) {
        if (strstr(wItems[i].name, query) != NULL) {
            printf("\n%d.\t %-20s %-20s %-20d", i, wItems[i].tags, wItems[i].name, wItems[i].price);
        }
    }
}

void searchProductByTags(warehouse wItems[]) {
    char query[100];

    printf("\nEnter tag to search: ");
    scanf(" %[^\n]", query);

    printf("Search Results:\n");
    for (int i = 0; i < warehouseCount; i++) {
        if (strstr(wItems[i].tags, query) != NULL) {
            printf("\n%d.\t %-20s %-20s %-20d", i, wItems[i].tags, wItems[i].name, wItems[i].price);
        }
    }
}
//=====================================================================================

//CART
//=====================================================================================
void cartMenu (int mustPay){
    clearScreen();
    printCart(cItems, wItems, mustPay);

    printf("\n1.\t CHECK OUT");
    printf("\n99.\t BACK");
    printf("\nSelect Menu : ");
    scanf("%d", &sw);
    clearScreen();
    switch (sw) {
        case 1:
            chooseDelivery(mustPay);
            break;
        case 99:
            repeat = 0;
            buyerMenu();
        default:
            printf("Invalid Menu");
            pressEnter ();
            break;
    }
    
    return;

}

void printCart(cart cItems[], warehouse wItems[], int mustPay) {
    int found;
    int totalPrice;

    printf("\n--------------------------- SHOPPING CART ---------------------------");
    printf("\n%-20s %-20s %-10s %-20s", "Name", "Price", "Amount", "Total");
    for (int i = 0; i < cartCount; i++) {
        
        found = 0;
        totalPrice = 0;

        for (int j = 0; j < warehouseCount; j++) {
            if (cItems[i].code == wItems[j].itemCode) {
                totalPrice = cItems[i].amount * wItems[j].price;
                printf("\n%-20s %-20d %-10d %-20d", wItems[j].name, wItems[j].price, cItems[i].amount, totalPrice);
                cItems[i].cWeight = wItems[j].weight;
                mustPay += totalPrice;
                found = 1;
                break; 
            }
        }

        if (!found) {
            printf("%-10ld %-20s\n", cItems[i].code, "Not Found");
        }
    }
    printf("\n---------------------------------------------------------------------");
    printf ("\nTOTAL: %d", mustPay);
    printf("\n---------------------------------------------------------------------");
}

void printInvoice(cart cItems[], warehouse wItems[], id user[], int chooseDelivery, int mustPay) {
    int grandTotal = 0;
    char choose;
    int tempBought = 0;
    int tempStock = 0;


    clearScreen();
    
    printf("\n------------------------------ INVOICE ------------------------------");
    
    printCart(cItems, wItems, mustPay);
    printf("\nAddress: %s, %s", address[user[nowUsingCode].userAdressCode].province, address[user[nowUsingCode].userAdressCode].city);

    char *invoiceArrival = countETA(address, courier, chooseDelivery);
    int invoiceShipCost = countShipmentCost(cItems, wItems, courier, chooseDelivery);
    printf("\nCourier: %s \tETA: %s \tShipping Cost: %d", courier[chooseDelivery].provider, invoiceArrival, invoiceShipCost);

    mustPay = 0;
    int totalPrice;

    for (int i = 0; i < cartCount; i++) {
        
        totalPrice = 0;

        for (int j = 0; j < warehouseCount; j++) {
            if (cItems[i].code == wItems[j].itemCode) {
                totalPrice = cItems[i].amount * wItems[j].price;
                cItems[i].cWeight = wItems[j].weight;
                tempBought = wItems[j].bought + cItems[i].amount;
                wItems[j].bought = tempBought;
                tempStock = wItems[j].stock - cItems[i].amount;
                wItems[j].stock = tempStock;
                mustPay += totalPrice;
                break; 
            }
        }
    }

    grandTotal = mustPay + invoiceShipCost;
    printf("\n---------------------------------------------------------------------");
    printf("\n\t\t\t\t\t\tGRAND TOTAL: %d", grandTotal);
    printf("\n---------------------------------------------------------------------");

    printf ("\nBALANCE: %d", user[nowUsingCode].balance);
    printf ("\nPAY (Y/N): ");
    scanf ("%s", &choose);
    if (choose == 'Y' || choose == 'y') {
        if (grandTotal <= user[nowUsingCode].balance) {
            user[nowUsingCode].balance -= grandTotal;
            saveInvoiceToFile(cItems, wItems, user, chooseDelivery, mustPay);
            updateUser();
            updateWarehouse();
            updateLog(cItems, wItems, user);
            clearCart();
            pressEnter();
        }
        else {
            printf ("\nINSUFFICIENT BALANCE");
            buyerMenu();
        }
    }
    else{
        buyerMenu();
    }
    
}

void saveInvoiceToFile(cart cItems[], warehouse wItems[], id user[], int chooseDelivery, int mustPay) {
    char filename[100];

    snprintf(filename, sizeof(filename), "%s_Invoice_%d.txt", date, user[nowUsingCode].userCode);
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating invoice file.\n");
        return;
    }

    
    fprintf(file, "------------------------------ INVOICE ------------------------------\n");
    fprintf(file, "\t\t\t\t\t\t%s", date);
    fprintf(file, "\n--------------------------- SHOPPING CART ---------------------------\n");
    fprintf(file, "%-20s %-20s %-10s %-20s\n", "Name", "Price", "Amount", "Total");
    
    int totalPrice;
    for (int i = 0; i < cartCount; i++) {
        totalPrice = 0;
        for (int j = 0; j < warehouseCount; j++) {
            if (cItems[i].code == wItems[j].itemCode) {
                totalPrice = cItems[i].amount * wItems[j].price;
                fprintf(file, "%-20s %-20d %-10d %-20d\n", wItems[j].name, wItems[j].price, cItems[i].amount, totalPrice);
                break;
            }
        }
    }
    
    fprintf(file, "---------------------------------------------------------------------\n");
    fprintf(file, "TOTAL: %d\n", mustPay);
    fprintf(file, "---------------------------------------------------------------------\n");
    
    fprintf(file, "Address: %s, %s\n", address[user[nowUsingCode].userAdressCode].province, address[user[nowUsingCode].userAdressCode].city);
    
    char *invoiceArrival = countETA(address, courier, chooseDelivery);
    int invoiceShipCost = countShipmentCost(cItems, wItems, courier, chooseDelivery);
    fprintf(file, "Courier: %s \tETA: %s \tShipping Cost: %d\n", courier[chooseDelivery].provider, invoiceArrival, invoiceShipCost);
    
    int grandTotal = mustPay + invoiceShipCost;
    fprintf(file, "---------------------------------------------------------------------\n");
    fprintf(file, "\t\t\t\t\tGRAND TOTAL: %d\n", grandTotal);
    fprintf(file, "---------------------------------------------------------------------\n");

    fclose(file);
    printf("\nInvoice saved to %s\n", filename);
}
//=====================================================================================

//SHIPMENT
//=====================================================================================
void chooseDelivery(int mustPay) {

    printDeliveryList();

    int chooseDelivery = 0;
    
    printf("\nSelect Menu (1-%d): ", deliveryCount);
    scanf ("%d", &chooseDelivery);
    chooseDelivery--;

    printInvoice(cItems,wItems, user, chooseDelivery, mustPay);
}

void printDeliveryList (){
    int count = 0;
    int shipCost;

    if (user[userCount].userAdressCode < 0 || user[userCount].userAdressCode >= addressCount) {
        printf("Invalid address code.\n");
        return;
    }

    printf("\n------------------------------ COURIER ------------------------------");
    printf("\n%s", address[user[nowUsingCode].userAdressCode].city);
    printf("\n%-20s %-20s %-20s\n", "Courier", "ETA", "Price");
    
    for (count = 0; count < deliveryCount; count++) {
        char *arrival = countETA(address, courier, count);
        shipCost = countShipmentCost(cItems, wItems, courier, count);
        printf("%-20s %-20s %-20d\n", courier[count].provider, arrival, shipCost);
    }

}

int countShipmentCost(cart cItems[], warehouse wItems[], delivery courier[], int count) {
    int totalWeight = 0;
    for (int i = 0; i < deliveryCount; i++){
        totalWeight += cItems[i].cWeight;
    }
    return totalWeight * courier[count].charge * address[nowUsingCode].distance / 100;
}

char* countETA(maps address[], delivery courier[], int count) {
    invoiceETASecond = 0;

    time_t currentTime = time(NULL);

    int addDay = (address[user[userCount].userAdressCode].distance / courier[count].speed) * 60 * 60;
    invoiceETASecond = currentTime + addDay;
    currentTime += addDay;

    struct tm *arrivalTime = localtime(&currentTime);
    static char arrival[11];
    
    sprintf(arrival, "%02d-%02d-%d",
            arrivalTime->tm_mday, arrivalTime->tm_mon + 1, arrivalTime->tm_year + 1900);

    return arrival;
}
//=====================================================================================

//SELLER
//=====================================================================================
void sellerMenu() {
    repeat = 1;
    readAll();
    sw = 0;

    do {
        clearScreen();
        printf("\n%s----------------------------- MAIN MENU -----------------------------%s", GREEN, NORM);
        printf("\n%s1.\t ADD STOCK", LIGHTGREEN);
        printf("\n2.\t NEW ITEMS");
        printf("\n3.\t CHECK RETURNED ITEMS");
        printf("\n%s99.\t LOG OUT", RED);
        printf("\n%sSelect Menu:%s ", BLUE, LIGHTBLUE);
        if(scanf("%d", &sw) != 1) {
            while(getchar() != '\n');
        }
        clearScreen();
        switch (sw) {
            case 1:
                addStock();
                break;
            case 2:
                addNewItem();
                break;
            case 3:
                confirmReturn();
                updateReturn();
                break;
            case 99:
                mainMenu();
                break;
            default:
                printf("Invalid Menu");
                break;
        }
    } while (repeat == 1);
}

void addStock() {
    char itemName[100];
    int amount;

    printf("\n%s----------------------------- ADD STOCK -----------------------------\n", GREEN);
    printf("%sEnter item name:%s ",MAGENTA, LIGHTBLUE);
    scanf(" %[^\n]", itemName);

    int found = 0;
    for (int i = 0; i < warehouseCount; i++) {
        if (strcmp(wItems[i].name, itemName) == 0) {
            printf("%sItem found:%s %s\n", MAGENTA,LIGHTBLUE, wItems[i].name);
            printf("%sCurrent stock:%s %d\n",MAGENTA, LIGHTBLUE, wItems[i].stock);
            printf("%sEnter amount to add:%s ",MAGENTA, LIGHTBLUE);
            scanf("%d", &amount);

            if (amount > 0) {
                wItems[i].stock += amount;
                printf("%sStock updated successfully! New stock:%s %d\n",GREEN, LIGHTGREEN, wItems[i].stock);
                updateWarehouse();
            } else {
                printf("%sInvalid amount. Please enter a positive number.%s\n", RED, NORM);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("%sItem not found.%s\n", RED, NORM);
    }

    pressEnter();
}

void addNewItem() {
    warehouse newItem;

    printf("\n%s----------------------------- NEW ITEM -----------------------------\n", GREEN);
    printf("%sEnter item details:\n",MAGENTA );

    newItem.itemCode = time(NULL);

    printf("Category:%s ", LIGHTBLUE);
    scanf(" %[^\n]", newItem.category);

    printf("%SName:%s ", MAGENTA, LIGHTBLUE);
    scanf(" %[^\n]", newItem.name);

    printf("%sPrice:%s ", MAGENTA, LIGHTBLUE);
    scanf("%d", &newItem.price);

    printf("%sStock:%s ", MAGENTA, LIGHTBLUE);
    scanf("%d", &newItem.stock);

    printf("%sWeight:%s ", MAGENTA, LIGHTBLUE);
    scanf("%d", &newItem.weight);

    printf("%sTags:%s ", MAGENTA, LIGHTBLUE);
    scanf(" %[^\n]", newItem.tags);

    newItem.bought = 0; // Initialize bought to 0

    if (warehouseCount < MAX_ITEMS) {
        wItems[warehouseCount++] = newItem;
        printf("%sNew item added successfully!%s\n", GREEN, NORM);
        updateWarehouse();
    } else {
        printf("%sWarehouse is full. Cannot add more items.%s\n", ORANGE, NORM);
    }

    pressEnter();
}

void confirmReturn() {

    char whichReturn[50];
    int returnIndex = 1;

    printf("\n%s--------------------------- RETURN REQUEST ---------------------------\n", GREEN);
    for(int i = 0; i < returnCount; i++) {
        if (rItems[i].checkReturn == 1 || rItems[i].checkReturn == 0) {
            printf("%sItem:%s %s\n",MAGENTA, LIGHTBLUE, rItems[i].itemName);
            printf("%sAmount:%s %d\n",MAGENTA, LIGHTBLUE, rItems[i].amount);
            printf("%sReturn status:%s ",YELLOW, NORM);
            if (rItems[i].confirmReturn == 1) {
                printf("%sConfirmed%s\n", GREEN, NORM);
            } else if (rItems[i].confirmReturn == 0) {
                printf("%sNot Confirmed%s\n", RED, GREEN);
            }
            printf("====================================\n");
        } else {
            printf("%sNo return request found.%s\n", RED, NORM);
            return;
        }
    }

    printf("\n%sEnter the name of the item you want to confirm (or %s'press N to return'):%s ",BLUE, ORANGE, LIGHTBLUE);
    scanf(" %[^\n]", whichReturn);

    if (toupper(whichReturn[0]) == 'N' && whichReturn[1] == '\0') {
        return; // return
    }

    do {
        printf("%sDo you wish to return the item? (Y/N):%s ", BLUE, LIGHTBLUE);
        int wChoice;
        scanf(" %c", &choice);

        for (int i = 0; i < warehouseCount; i++) {
            if (strcmp(wItems[i].name, whichReturn) == 0) {
                wChoice = i;
            }
        }

        do {
            if (toupper(choice) == 'Y') {
                for (int i = 0; i < returnCount; i++) {
                    if (strcmp(rItems[i].itemName, whichReturn) == 0) {
                        rItems[i].confirmReturn = 1;
                        rItems[i].checkReturn = 1;

                        wItems[wChoice].stock += rItems[i].amount;
                        user[nowUsingCode].balance -= (wItems[wChoice].price * rItems[i].amount);

                        for(int j = 0; j < userCount; j++) {
                            if (user[j].userCode == rItems[i].userReturn) {
                                user[0].balance -= (wItems[wChoice].price * rItems[i].amount);
                                user[j].balance += (wItems[wChoice].price * rItems[i].amount);
                            }
                        }

                        updateWarehouse();
                        updateUser();
                        updateReturn();
                        readAll();

                        printf("Return confirmed.\n");
                        returnIndex = 0;
                        break;
                    }
                }
            } else if (toupper(choice) == 'N') {
                printf("%SYou refused to return the item.\nWe will keep the item for the buyer.%s\n", RED, LIGHTBLUE);
                for (int i = 0; i < returnCount; i++) {
                    if (strcmp(rItems[i].itemName, whichReturn) == 0) {
                        rItems[i].confirmReturn = 0;
                        rItems[i].checkReturn = 1;
                        returnIndex = 0;
                        break;
                    }
                }
                returnIndex = 0;
            } else {
                printf("%sInvalid input. Please enter %s'Y' %sor %s'N'%s.\n", RED, GREEN, NORM, RED, NORM);
                returnIndex = 1;
            }
        } while (returnIndex == 1);
    } while (returnIndex == 1);

    readAll();

    hist addNewLog;
    for(int i = 0; i < returnCount; i++) {
        for( int j = 0; j < logCount; j++) {
            if (rItems[i].itemCode == logs[j].itemCode && rItems[i].userReturn == logs[j].userCode && strcmp(whichReturn, logs[j].itemName) == 0) {
                addNewLog.userCode = rItems[i].userReturn;
                addNewLog.dateBuyed = time(NULL);
                addNewLog.dateArrived = logs[j].dateArrived;
                addNewLog.amount = -rItems[i].amount;
                addNewLog.itemCode = rItems[i].itemCode;
                strcpy(addNewLog.itemName, rItems[i].itemName);
                logs[logCount++] = addNewLog;
                addLog();
                exit(0);
            }
        }
    }

    pressEnter();
}
//=====================================================================================
