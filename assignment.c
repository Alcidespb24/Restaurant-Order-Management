#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 6
#define TAX_RATE 0.07

struct order
{
    int orderId;
    char customerName[50];
    struct items *orderItems[MAX_ITEMS];
    int quantities[MAX_ITEMS];
    int itemCount;
    float total;
};

struct items
{
    char *itemName;
    float price;
};

struct items menu[MAX_ITEMS] =
{
    {"Burger", 4.99},
    {"Sandwich", 4.49},
    {"Tenders", 3.99},
    {"Fries", 1.99},
    {"Cookies", 1.00},
    {"Soft Drink", 0.89}
};

void menuDisplay();
void takeOrder(struct order *ord);
void orderConfirmation(struct order *ord);
void writeOrder(struct order *ord);
void saveSummary(struct order *orders, int orderCount);

int main()
{
    int selection;
    struct order orders[50];
    int orderCount = 0;

    menuDisplay();

    while(1)
    {
        printf("----------------------------------\n");
        printf("1 - I would like to make an order\n");
        printf("2 - I am done\n");
        scanf("%d", &selection);
        printf("----------------------------------\n");

        if(selection == 1)
        {
            orders[orderCount].orderId = orderCount + 1;
            takeOrder(&orders[orderCount]);
            orderConfirmation(&orders[orderCount]);
            writeOrder(&orders[orderCount]);
            orderCount++;
        }
        else if(selection == 2)
        {
            saveSummary(orders, orderCount);
            break;
        }
        else
        {
            printf("Select one of the options given, try again");
        }
    }

    return 0;
}

void menuDisplay()
{
    printf("Menu\n");
    printf("----------------------------------\n");
    for(int i = 0; i < MAX_ITEMS; i++)
    {
        printf("%d. %s - $%.2f\n", i + 1, menu[i].itemName, menu[i].price);
    }

}

void takeOrder(struct order *ord)
{
    int item, quantity;
    char buffer[50];

    printf("Enter customer name : ");
    scanf("%s", ord->customerName);

    ord->itemCount = 0;
    ord->total = 0.0;

    while(1)
    {
        printf("Enter the item number or 0 to finish : ");
        scanf("%d", &item);
        if(item == 0)break;

	if(item < 0 || item > MAX_ITEMS){
                printf("Please select one of the options in the menu\n");
                continue;
        }


        printf("Enter the quantity : ");
        scanf("%d", &quantity);

        ord->orderItems[ord->itemCount] = &menu[item - 1];
        ord->quantities[ord->itemCount] = quantity;
        ord->total += menu[item - 1].price * quantity;
        ord->itemCount++;
    }
    ord->total += ord->total * TAX_RATE;
}

void orderConfirmation(struct order *ord)
{
    printf("Order Summary for %s : \n", ord->customerName);
    for(int i = 0; i < ord->itemCount; i++)
    {
        printf("%d X %s - $%.2f\n", ord->quantities[i], ord->orderItems[i]->itemName, ord->orderItems[i]->price);
    }
    printf("Total, with taxes included : $%.2f\n", ord->total);
}

void writeOrder(struct order *ord)
{
    FILE *file = fopen("orders.txt", "a");
    if(file == NULL)
    {
        printf("The file could not be opened\n");
        return;
    }
    fprintf(file, "Order ID: %d\nCustomer Name : %s\n", ord->orderId, ord->customerName);
    for(int i = 0; i < ord->itemCount; i++)
    {
        fprintf(file, "%d X %s - $%.2f\n", ord->quantities[i], ord->orderItems[i]->itemName, ord->orderItems[i]->price);
    }
    fprintf(file, "Total, with taxes included : $%.2f\n", ord->total);

    fclose(file);
}

void saveSummary(struct order *orders, int orderCount)
{
    FILE *file = fopen("summary.txt", "w");
    if(file == NULL)
    {
        printf("The file could not be opened\n");
        return;
    }

    float totalRevenue = 0.0;
    int totalOrders = 0;
    int sales[MAX_ITEMS] = {0};
    float itemRevenue[MAX_ITEMS] = {0.0};

    for(int i = 0; i < orderCount; i++)
    {
        totalOrders++;
        totalRevenue += orders[i].total;
        for(int j = 0; j < orders[i].itemCount; j++)
        {
            for(int k = 0; k < MAX_ITEMS; k++)
            {
                if(strcmp(orders[i].orderItems[j]->itemName, menu[k].itemName) == 0)
                {
                    sales[k] += orders[i].quantities[j];
                    itemRevenue[k] += orders[i].orderItems[j]->price * orders[i].quantities[j];
                }
            }
        }
    }

    fprintf(file, "Total Orders : %d\n", totalOrders);
    fprintf(file, "Total Revenue : $%.2f\n", totalRevenue);
    for(int i = 0; i < MAX_ITEMS; i++){
        fprintf(file, "%s - Total Sales : %d\n Total Revenue : $%.2f\n", menu[i].itemName, sales[i], itemRevenue[i]);
    }

    fclose(file);

    printf("Sales Summary\n");
    printf("----------------------------------\n");
    printf("Total Orders : %d\n", totalOrders);
    printf("Total Revenue : $%.2f\n", totalRevenue);
    for(int i = 0; i < MAX_ITEMS; i++){
        printf("%s - Total Sales : %d, Total Revenue : $%.2f\n", menu[i].itemName, sales[i], itemRevenue[i]);
    }
}



