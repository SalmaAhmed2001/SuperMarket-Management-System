#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Product Product;
typedef struct Category  Category;

struct Product {
    char *name;
    float price;
    int quantity;
};

struct Category {
    char name[100];
    Product *prod;
    int prodNum;
};

//void displayCategories(Category *cat, int catNum);

void printMenu() {
    printf("\nWelcome To Salma SuperMarket:\n");
    printf("1. Add Category\n");
    printf("2. Add Product\n");
    printf("3. Display Categories\n");
    printf("4. Display Products\n");
    printf("5. Move Product\n");
    printf("6. Sell Product\n");
    printf("7. Exit\n");
}

void getUserInput(int *choice) {
    printf("Enter your choice (1-7): ");
    scanf("%d", choice);
    while (getchar() != '\n');
}

int isAlpha(char *str) {
    while (*str) {
        if (!isalpha((unsigned char)*str) && *str != ' ') {
            return 0;
        }
        str++;
    }
    return 1;
}

void clearInputBuffer() {
    int c;
    //getchar return number (the ASCII value of the character read)
    //define as negative intger const -> EOF
    while ((c = getchar()) != '\n' && c != EOF);
}
void addCategory(Category **cat, int *indx) {
    *cat = realloc(*cat, (*indx + 1) * sizeof(Category));

    if (*cat != NULL) {
        char categoryName[100];
        int isValidInput;

        do {
            printf("Enter the name of the category: ");
            isValidInput = 1;

            if (scanf("%99s", categoryName) != 1) {
                isValidInput = 0;
                printf("Invalid input. Please enter a valid category name.\n");
                clearInputBuffer();
            } else if (!isAlpha(categoryName)) {
                isValidInput = 0;
                printf("Invalid input. Category name should contain only alphabetic characters and spaces. Please enter again: ");
            } else {
                int duplicate = 0;
                for (int i = 0; i < *indx; i++) {
                    if (strcmp((*cat)[i].name, categoryName) == 0) {
                        duplicate = 1;
                        printf("Category with the same name already exists. Please enter a different name: ");
                        break;
                    }
                }
                if (!duplicate) {
                    strcpy((*cat)[*indx].name, categoryName);
                    printf("Category added successfully.\n");
                    (*cat)[*indx].prod = NULL;
                    (*cat)[*indx].prodNum = 0;
                    (*indx)++;
                }
            }
        } while (!isValidInput);
    } else {
        printf("Failed to add category. Memory allocation error.\n");
    }
}



void addProduct(Category *cat, int catNum) {
    if (catNum == 0) {
        printf("Please add a category first.\n");
        return;
    }

    int catIndex;

    printf("Select a category:\n");
    for (int i = 0; i < catNum; i++) {
        printf("%d. %s\n", i + 1, cat[i].name);
    }

    printf("Enter the number corresponding to the category: ");
    if (scanf("%d", &catIndex) != 1 || catIndex < 1 || catIndex > catNum) {
        printf("Invalid category selection.\n");
        clearInputBuffer();
        return;
    }

    catIndex--;

    int newProdNum;
    printf("Enter the number of products to add: ");
    if (scanf("%d", &newProdNum) != 1 || newProdNum <= 0) {
        printf("Invalid input. Please enter a valid number of products.\n");
        clearInputBuffer();
        return;
    }

    // Use a temporary pointer when reallocating
    Product *temp = realloc(cat[catIndex].prod, (cat[catIndex].prodNum + newProdNum) * sizeof(Product));
    if (temp == NULL) {
        printf("Memory allocation failed. Unable to add products.\n");
        return;
    }
    cat[catIndex].prod = temp;

    if (cat[catIndex].prod != NULL) {
        for (int i = 0; i < newProdNum; i++) {
            char productName[100];
            int isValidInput;

            do {
                printf("Enter product name: ");
                isValidInput = 1;

                if (scanf("%99s", productName) != 1 || strcspn(productName, "0123456789") != strlen(productName)) {
                    isValidInput = 0;
                    printf("Invalid input. Please enter a valid product name without numbers.\n");
                    clearInputBuffer();
                } else {
                    // Check for duplicate product names within the category and current session
                    int duplicate = 0;
                    for (int j = 0; j < cat[catIndex].prodNum; j++) {
                        if (strcmp(cat[catIndex].prod[j].name, productName) == 0) {
                            duplicate = 1;
                            printf("Product with the same name already exists in this category. Please enter a different name: ");
                            isValidInput = 0;
                            break;
                        }
                    }

                    for (int j = 0; j < cat[catIndex].prodNum + i; j++) {
                        if (strcmp(cat[catIndex].prod[j].name, productName) == 0) {
                            duplicate = 1;
                            printf("You have already entered a product with the same name in this session. Please enter a different name: ");
                            isValidInput = 0;
                            break;
                        }
                    }

                    if (!duplicate) {
                        do {
                            clearInputBuffer();

                            printf("Enter product price: ");
                            if (scanf("%f", &cat[catIndex].prod[cat[catIndex].prodNum + i].price) != 1 || cat[catIndex].prod[cat[catIndex].prodNum + i].price < 0) {
                                isValidInput = 0;
                                printf("Invalid input. Please press enter and renter a valid non-negative product price.\n");
                                clearInputBuffer();
                            } else {
                                isValidInput = 1;
                            }

                        } while (!isValidInput);

                        do {
                            clearInputBuffer();

                            printf("Enter product quantity: ");
                            if (scanf("%d", &cat[catIndex].prod[cat[catIndex].prodNum + i].quantity) != 1 || cat[catIndex].prod[cat[catIndex].prodNum + i].quantity < 0) {
                                isValidInput = 0;
                                printf("Invalid input. Please press enter and renter a valid non-negative product quantity.\n");
                                clearInputBuffer();
                            } else {
                                isValidInput = 1;
                            }

                        } while (!isValidInput);
                    }
                }
            } while (!isValidInput);

            // Allocate memory for the product name
            cat[catIndex].prod[cat[catIndex].prodNum + i].name = malloc(strlen(productName) + 1);
            strcpy(cat[catIndex].prod[cat[catIndex].prodNum + i].name, productName);
        }

        cat[catIndex].prodNum += newProdNum;

        printf("Products added to category '%s'.\n", cat[catIndex].name);
    } else {
        printf("Memory allocation failed. Unable to add products.\n");
    }
}


void displayCategories(Category *cat, int catNum) {
    if (catNum > 0) {
        printf("\nCategories:\n");
        for (int i = 0; i < catNum; i++) {
            printf("%d. %s\n", i + 1, cat[i].name);
        }
    } else {
        printf("No categories to display.\n");
    }
}
void displayProducts(Category *cat, int catNum) {
    if (catNum > 0) {
        int catIndx;

        printf("Select a category:\n");
        for (int i = 0; i < catNum; i++) {
            printf("%d. %s\n", i + 1, cat[i].name);
        }

        printf("Enter the number corresponding to the category: ");
        scanf("%d", &catIndx);

        if (catIndx < 1 || catIndx > catNum) {
            printf("Invalid category selection.\n");
            return;
        }

        catIndx--;

        Category *selectedCat = &cat[catIndx];

        if (selectedCat->prodNum > 0) {
            printf("\nProducts in Category '%s':\n", selectedCat->name);

            for (int i = 0; i < selectedCat->prodNum; i++) {
                if (selectedCat->prod[i].quantity > 0) {
                    printf("%d. %s Price: $%.2f Quantity: %d\n",
                           i + 1,
                           selectedCat->prod[i].name,
                           selectedCat->prod[i].price,
                           selectedCat->prod[i].quantity);
                }
            }
        } else {
            printf("No products in this category.\n");
        }
    } else {
        printf("No categories to display.\n");
    }
}

void moveProduct(Category *cat, int catNum) {
    if (catNum < 2) {
        printf("At least two categories are required.\n");
        return;
    }

    int srcindx, destindx, prodIndx;

    printf("Select the source category:\n");
    for (int i = 0; i < catNum; i++) {
        printf("%d. %s\n", i + 1, cat[i].name);
    }
    printf("Enter the number of the source: ");
    scanf("%d", &srcindx);

    if (srcindx < 1 || srcindx > catNum) {
        printf("Invalid selection.\n");
        return;
    }
    srcindx--;

    Category *srcCat = &cat[srcindx];

    if (srcCat->prodNum == 0) {
        printf("No products in the source category.\n");
        return;
    }

    printf("Select the destination category:\n");
    for (int i = 0; i < catNum; i++) {
        if (i != srcindx) {
            printf("%d. %s\n", i + 1, cat[i].name);
        }
    }
    printf("Enter the number of destination category: ");
    scanf("%d", &destindx);

    if (destindx < 1 || destindx > catNum || destindx == srcindx + 1) {
        printf("Invalid destination category selection.\n");
        return;
    }
    destindx--;

    printf("Select the product to move:\n");
    for (int i = 0; i < srcCat->prodNum; i++) {
        printf("%d. %s\n", i + 1, srcCat->prod[i].name);
    }
    printf("Enter the number of the product: ");
    scanf("%d", &prodIndx);

    if (prodIndx < 1 || prodIndx > srcCat->prodNum) {
        printf("Invalid product selection.\n");
        return;
    }
    prodIndx--;

    Product movedProd = srcCat->prod[prodIndx];
    srcCat->prodNum--;
    srcCat->prod[prodIndx] = srcCat->prod[srcCat->prodNum];

    cat[destindx].prod = realloc(
        cat[destindx].prod,
        (cat[destindx].prodNum + 1) * sizeof(Product)
    );

    if (cat[destindx].prod != NULL) {
        cat[destindx].prod[cat[destindx].prodNum] = movedProd;
        cat[destindx].prodNum++;

        printf("Product '%s' moved from '%s' to '%s' successfully!\n",
               movedProd.name, srcCat->name, cat[destindx].name);
    } else {
        printf("Allocation failed. Unable to move the product.\n");
    }
}
void sellProduct(Category *cat, int catNum) {
    if (catNum == 0) {
        printf("Please add a category first.\n");
        return;
    }

    int catIndx;

    printf("Select a category to sell a product from.\n");
    for (int i = 0; i < catNum; i++) {
        printf("%d. %s\n", i + 1, cat[i].name);
    }

    printf("Enter the number corresponding to the category: ");
    scanf("%d", &catIndx);

    if (catIndx < 1 || catIndx > catNum) {
        printf("Invalid category selection.\n");
        return;
    }

    catIndx--;

    if (cat[catIndx].prodNum == 0) {
        printf("No products in this category to sell.\n");
        return;
    }

    int prodIndx;

    printf("Select the product to sell.\n");
    for (int i = 0; i < cat[catIndx].prodNum; i++) {
        printf("%d. %s\n", i + 1, cat[catIndx].prod[i].name);
    }

    printf("Enter the number of the product to sell: ");
    scanf("%d", &prodIndx);

    if (prodIndx < 1 || prodIndx > cat[catIndx].prodNum) {
        printf("Invalid product selection.\n");
        return;
    }

    prodIndx--;

    printf("Enter the quantity to sell: ");

    int sellQuantity;
    scanf("%d", &sellQuantity);

    if (sellQuantity <= 0 || sellQuantity > cat[catIndx].prod[prodIndx].quantity) {
        printf("Invalid quantity to sell.\n");
        return;
    }

    float totalPrice = cat[catIndx].prod[prodIndx].price * sellQuantity;


    printf("Total price for sold quantity: $%.2f\n", totalPrice);
    printf("Do you want to proceed with the sale? (1. Yes / 2. No): ");

    int confirm;
    scanf("%d", &confirm);

    if (confirm == 1) {
        cat[catIndx].prod[prodIndx].quantity -= sellQuantity;
        printf("Sold %d units of '%s' from category '%s' for a total price of $%.2f.\n",
               sellQuantity, cat[catIndx].prod[prodIndx].name, cat[catIndx].name, totalPrice);
    } else {
        printf("Sale canceled.\n");
    }
}


int main() {
    Category *cat = NULL;
    int catNum = 0;
    int choice;

    do {

        printMenu();
        getUserInput(&choice);

        switch (choice) {
            case 1:
                addCategory(&cat, &catNum);
                break;
            case 2:
                addProduct(cat, catNum);
                break;
            case 3:
                displayCategories(cat, catNum);
                break;
            case 4:
                displayProducts(cat, catNum);
                break;
            case 5:
                moveProduct(cat, catNum);
                break;
            case 6:
                sellProduct(cat, catNum);
                break;
            case 7:
                printf("Goodbye.\n");
                break;
            default:
                printf("Invalid choice. Please re-enter a valid option.\n");
        }
    } while (choice != 7);


      for (int i = 0; i < catNum; i++) {
        free(cat[i].prod);
    }
    free(cat);


    return 0;
}
