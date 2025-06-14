#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
	int day;
	int month;
	int year;
} Date;

typedef struct {
	Date HSX;
	Date HSD;
} Expiry;

typedef struct {
	char employeeCode[20];
	char password[50];
} Account;

typedef struct Item {
	char name[100];
	int barcode;
	char category[100];
	int price;
	int stockQuantity;
	char description[200];
	char supplier[100];
	Expiry expiryData;
	char unit[20];
} Item;
typedef struct Node {
	Item item;
	struct Node *next;
} Node;

typedef struct List {
	Node *head;
	Node *tail;
} List;

typedef struct tab {
	short name;
	short barcode;
	short category;
	short price;
	short stockQuantity;
	short description;
	short supplier;
	short HSX;
	short HSD;
	short unit;
} Tab;

typedef struct bill {
	List listBill;
	int number;
} Bill;

// Node
void ds(List *ds);
Node *createNode(Item item);
void changeItem(List *ds, Node *temp1, Node *temp2);
void addFistList(List *ds, Item item);
// hoan doi temp1 cho temp2
// item
typedef char *(*ChooseChar)(Item *item);
typedef int (*ChooseNumber)(Item *item);
int choosePrice(Item *item);
int chooseStock(Item *item);
int chooseDate(Item *item);
char *chooseName(Item *item);
char *chooseCategory(Item *item);
char *chooseSupplier(Item *item);
char *chooseUnit(Item *item);
void sortItem(List *ds, ChooseChar chooseChar, int inc, ChooseNumber chooseNumber, short HSXorHSD);
void addItem(List *ds, char *itemInfo);
int checkWhileSpace(char a);
char *deleteWhiteSpace(char *Text);
void saveDateHSX(Expiry *expiry, char *HSX);
void saveDateHSD(Expiry *expiry, char *HSD);
Item inputItem();

// menu
void tableHide(List ds, Tab tab);
void tabSet(Tab *tab);
void arrange(List *ds, int choose, int inc);
int searchItemByName(List *ds, char *name, List *searchList);
void addItemToListBuy(List *list, int quantity, int barcode, List *listBuy);
List menuBuy(List *ds);
void menu(List *ds);
void menuText(int i, int fontMenu);
void menuBanHang(List *ds, Bill *bill);
void menuQuanLy(List *ds);

void passMenu();
void checkPass(int *i, Account *account, int number);

int checkBuy(char *text);
int checkBarcode(List *ds, Item *item);
void tableBuy(List ds, Tab tab);
int checkListBuy(List *ds, Item item);
void showBill(List ds, Tab tab, int number);
int sumBill(List bill);

int main() {
	passMenu();
	List listItem;
	Tab tab;
	tabSet(&tab);
	ds(&listItem);
	int i = 1;

	FILE *f;
	f = fopen("items.txt", "r");
	printf(">> Nhap file du lieu:\n");
	printf("> items.txt\n");
	printf("> Password.txt\n");
	char itemInfo[512];

	while (fgets(itemInfo, sizeof(itemInfo), f)) {

		if (itemInfo[strlen(itemInfo) - 1] == '\n') {
			itemInfo[strlen(itemInfo) - 1] = '\0';
		}
		if (i != 1) {

			addItem(&listItem, itemInfo);
		}
		i++;
	}

	fclose(f);
	menu(&listItem);
	return 0;
}

void menu(List *ds) {
	int choice = 1;
	int fontMenu = 1;
	int maxChoice = 2;
	Bill bill[20];
	for (int i = 0; i < 20; i++) {
		bill[i].number = 0;
	}
	do {
		do {
			system("cls");
			menuText(choice, fontMenu);

			int a = _getch();

			if (a == 0 || a == 224) {
				int b = _getch();
				if (b == 77) {
					choice++;
					if (choice > maxChoice) {
						choice = 0;
					}
					system("cls");
					menuText(choice, fontMenu);
				} else if (b == 75) {
					if (choice == 0) {
						choice = maxChoice;
						system("cls");
						menuText(choice, fontMenu);
					} else {
						choice--;
						system("cls");
						menuText(choice, fontMenu);
					}
				}
			} else if (a >= '0' && a <= '9') {
				int num = a - '0';
				if (num >= 0 && num <= maxChoice) {
					choice = num;
					system("cls");
					menuText(choice, fontMenu);
				}
			} else if (a == 13) {
				break;
			}
		} while (1);

		switch (choice) {
			case 1:
				menuBanHang(ds, bill);
				break;
			case 2:
				menuQuanLy(ds);
				break;
			case 0:
				return;
		}
	} while (1);
}

void menuQuanLy(List *dS) {
	int choice = 1;
	int fontMenu = 3;
	int maxChoice = 4;
	Tab tab;
	tabSet(&tab);

	do {
		do {
			system("cls");
			menuText(choice, fontMenu);

			int a = _getch();

			if (a == 0 || a == 224) {
				int b = _getch();
				if (b == 77) {
					choice++;
					if (choice > maxChoice) {
						choice = 0;
					}
					system("cls");
					menuText(choice, fontMenu);
				} else if (b == 75) {
					if (choice == 0) {
						choice = maxChoice;
						system("cls");
						menuText(choice, fontMenu);
					} else {
						choice--;
						system("cls");
						menuText(choice, fontMenu);
					}
				}

			} else if (a >= '0' && a <= '9') {
				int num = a - '0';
				if (num >= 0 && num <= maxChoice) {
					choice = num;
					system("cls");
					menuText(choice, fontMenu);
				}
			} else if (a == 13) {
				break;
			}
		} while (1);

		switch (choice) {
			case 1: {
				Item item = inputItem();
				addFistList(dS, item);
				printf("> Da them thanh cong!\n");
				printf("> Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n');
				break;
			}

			case 2: {
				system("cls");
				char name[100];
				List searchList;
				ds(&searchList);
				printf(">> Tim kiem san pham: \n");
				printf("> Nhap ten can tim kiem: \n");
				fgets(name, sizeof(name), stdin);
				name[strcspn(name, "\n")] = '\0';

				int found = searchItemByName(dS, name, &searchList);
				if (found > 0) {
					printf("> San pham da tim: \n");
					tableHide(searchList, tab);
				} else {
					printf("> Khong tim thay san pham nao het!\n");
					printf("> Nhan Enter de tiep tuc...\n");
					while (getchar() != '\n');

				}
				break;
			}


			case 3: {

				int choice1 = 1;
				int fontMenu1 = 4;
				int maxChoice1 = 8;

				do {
					system("cls");
					menuText(choice1, fontMenu1);

					int a = _getch();
					if (a == 0 || a == 224) {
						int b = _getch();
						if (b == 77) {
							choice1++;
							if (choice1 > maxChoice1)
								choice1 = 0;
						} else if (b == 75) {
							if (choice1 == 0)
								choice1 = maxChoice1;
							else
								choice1--;
						}
					} else if (a >= '0' && a <= '9') {
						int num = a - '0';
						if (num >= 0 && num <= maxChoice1) {
							choice1 = num;
							system("cls");
							menuText(choice1, fontMenu);
						}
					} else if (a == 13) {
						break;
					}
				} while (1);

				int choice2 = 1;
				int fontMenu2 = 5;
				int maxChoice2 = 1;

				do {
					system("cls");
					menuText(choice2, fontMenu2);

					int a = _getch();
					if (a == 0 || a == 224) {
						int b = _getch();
						if (b == 77) {
							choice2++;
							if (choice2 > maxChoice2)
								choice2 = 0;
						} else if (b == 75) {
							if (choice2 == 0)
								choice2 = maxChoice2;
							else
								choice2--;
						}
					} else if (a >= '0' && a <= '9') {
						int num = a - '0';
						if (num >= 0 && num <= maxChoice2) {
							choice2 = num;
							system("cls");
							menuText(choice2, fontMenu);
						}
					} else if (a == 13) {
						break;
					}
				} while (1);

				arrange(dS, choice1, choice2);
				printf("Da sap xep!\n");
				printf("> Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n')

					break;
			}
			case 4:
				printf(">> Kho chua: \n");
				tableHide(*dS,tab);
				printf("> Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n')

				case 0:
				return;
		}
	} while (1);
}

void menuBanHang(List *dS, Bill *bill) {
	int choice = 1;
	int fontMenu = 2;
	int maxChoice = 3;
	Tab tab;
	tabSet(&tab);
	do {
		do {
			system("cls");
			menuText(choice, fontMenu);

			int a = _getch();

			if (a == 0 || a == 224) {
				int b = _getch();
				if (b == 77) {
					choice++;
					if (choice > maxChoice) {
						choice = 0;
					}
					system("cls");
					menuText(choice, fontMenu);
				} else if (b == 75) {
					if (choice == 0) {
						choice = maxChoice;
						system("cls");
						menuText(choice, fontMenu);
					} else {
						choice--;
						system("cls");
						menuText(choice, fontMenu);
					}
				}
			} else if (a >= '0' && a <= '9') {
				int num = a - '0';
				if (num >= 0 && num <= maxChoice) {
					choice = num;
					system("cls");
					menuText(choice, fontMenu);
				}
			} else if (a == 13) {
				break;
			}
		} while (1);

		switch (choice) {
			case 1:
				system("cls");
				tableHide(*dS, tab);
				printf(">> Nhap san pham: \n");
				List billtemp = menuBuy(dS);
				printf("1");
				for (int i = 0; i < 20; i++) {
					printf("2");
					if ((bill + i)->number == 0) {
						(bill + i)->listBill = billtemp;
						break;
					}
				}

				break;
			case 2:
				system("cls");
				char name[100];
				List searchList;
				ds(&searchList);
				printf(">> Tim kiem san pham: \n");
				printf("> Nhap ten can tim kiem: \n");
				fgets(name, sizeof(name), stdin);
				name[strcspn(name, "\n")] = '\0';

				int found = searchItemByName(dS, name, &searchList);
				if (found > 0) {
					printf("> Ban co muon mua san pham nao?\n");
					printf(">> Nhap san pham: \n");
					tableHide(searchList, tab);
					List billtemp = menuBuy(dS);
					for (int i = 0; i < 20; i++) {
						if ((bill + i)->number == 0) {
							(bill + i)->listBill = billtemp;
							(bill + i)->number = i + 1;
							break;
						}
					}
				} else {
					printf("> Khong tim thay duoc  san pham\n");
					printf("Nhan Enter de tiep tuc...\n");
					while (getchar() != '\n')
						;
				}

				break;
			case 3:
				system("cls");
				int count = 1;
				for (int i = 0; i < 20; i++) {
					if ((bill + i)->number != 0) {
						count++;
					}
				}
				printf(">> Xem lai bill cu: ");
				for (int i = 0; i < count; i++) {
					printf("> Bill %d: %d\n", i + 1, sumBill((bill + i)->listBill));
				}
				int number;
				printf("> Chon bill so: ");
				scanf("%d", &number);
				getchar();
				showBill((bill + number - 1)->listBill, tab, number);
				printf("Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n')
					;
				break;
			case 0:
				return;
		}
	} while (1);

}

void menuText(int i, int fontMenu) {

	switch (fontMenu) {

		case 1:
			printf("\n=========== MENU CUA HANG ===========\n");
			printf("%s1. Ban hang\n", i == 1 ? "> " : "");
			printf("%s2. Quan ly cua hang\n", i == 2 ? "> " : "");
			printf("%s0. Thoat\n", i == 0 ? "> " : "");
			printf("====================================\n");
			break;
		case 2:
			printf("\n=========== MENU BAN HANG ===========\n");
			printf("%s1. Nhap vat pham\n", i == 1 ? "> " : "");
			printf("%s2. Tim kiem vat pham\n", i == 2 ? "> " : "");
			printf("%s3. Danh sach bill cu\n", i == 3 ? "> " : "");
			printf("%s0. Quay tro lai\n", i == 0 ? "> " : "");
			printf("====================================\n");
			break;
		case 3:
			printf("\n=========== MENU QUAN LY ===========\n");
			printf("%s1. Them san pham vao kho\n", i == 1 ? "> " : "");
			printf("%s2. Tim kiem vat pham\n", i == 2 ? "> " : "");
			printf("%s3. Xap sep san pham\n", i == 3 ? "> " : "");
			printf("%s4. Kho\n", i == 4 ? "> " : "");
			printf("%s0. Quay tro lai\n", i == 0 ? "> " : "");
			printf("====================================\n");
			break;
		case 4:
			printf("\n======= CHON TIEU CHI SAP XEP =======\n");
			printf("%s1. Ten\n", i == 1 ? "> " : "  ");
			printf("%s2. The loai\n", i == 2 ? "> " : "  ");
			printf("%s3. Gia\n", i == 3 ? "> " : "  ");
			printf("%s4. So luong\n", i == 4 ? "> " : "  ");
			printf("%s5. Nha san xuat\n", i == 5 ? "> " : "  ");
			printf("%s6. Ngay san xuat\n", i == 6 ? "> " : "  ");
			printf("%s7. Han su dung\n", i == 7 ? "> " : "  ");
			printf("%s8. Don vi\n", i == 8 ? "> " : "  ");
			printf("%s0. Quay tro lai\n", i == 0 ? "> " : "  ");
			printf("=====================================\n");
			break;
		case 5:
			printf("\n========= CHON THU TU SAP XEP =========\n");
			printf("%s1. Tang dan\n", i == 1 ? "> " : "  ");
			printf("%s0. Giam dan\n", i == 0 ? "> " : "  ");
			printf("=======================================\n");
			break;
	}

	printf("> Su dung enter lua chon \n");
	printf("> Su dung dau mui ten de dieu huong(<-) (->)\n");
}

List menuBuy(List *list) {
	List listBuy;
	ds(&listBuy);

	while (1) {
		Item item;
		while (1) {
			printf("Nhap ma barcode: ");
			int result = scanf("%d", &item.barcode);
			getchar();
			if (result != 1) {
				printf("Vui long nhap so nguyen hop le!\n");
				int ch;
				while ((ch = getchar()) != '\n' && ch != EOF);
				continue;
			}

			if (checkBarcode(list, &item) == 1) {
				break;
			}
		}
		while (1) {
			printf("Nhap so luong: ");
			int result = scanf("%d", &item.stockQuantity);
			getchar();
			if (result != 1 || item.stockQuantity <= 0) {
				printf("Vui long nhap so luong la so nguyen duong!\n");
				int ch;
				while ((ch = getchar()) != '\n' && ch != EOF);
				continue;
			}

			break;
		}
		if (checkListBuy(&listBuy, item) != 1) {
			addFistList(&listBuy, item);
		}

		printf("> Bam phim bat ki de tiep tuc!\n");
		printf("> Bam dau mui ten (->) de thoat!\n");

		int a = _getch();

		if (a == 0 || a == 224) {
			int b = _getch();
			if (b == 77) {
				printf("> Da chon xong mon\n");
				break;
			}
		}
	}
	Tab tab;
	tabSet(&tab);

	tableBuy(listBuy, tab);
	printf("Nhan Enter de tiep tuc...\n");
	while (getchar() != '\n');

	if (checkBuy("thanh toan hoa don") != 1) {
		printf("Huy hoa don.\n");
		List emptyList;
		ds(&emptyList);
		return emptyList;
	}

	for (Node *q = listBuy.head; q != NULL; q = q->next) {
		for (Node *p = list->head; p != NULL; p = p->next) {
			if (q->item.barcode == p->item.barcode) {
				p->item.stockQuantity -= q->item.stockQuantity;
			}
		}
	}

	return listBuy;
}

void addItemToListBuy(List *list, int quantity, int barcode, List *listBuy) {
	for (Node *temp1 = list->head; temp1 != NULL; temp1 = temp1->next) {
		if (temp1->item.barcode == barcode) {
			addFistList(listBuy, temp1->item);
		}
	}
}

int searchItemByName(List *ds, char *name, List *searchList) {
	int found = 0;
	char nameTemp1[100];
	strcpy(nameTemp1, name);
	strlwr(nameTemp1);
	for (Node *temp1 = ds->head; temp1 != NULL; temp1 = temp1->next) {

		char nameTemp2[100];
		strcpy(nameTemp2, temp1->item.name);
		strlwr(nameTemp2);
		if (strstr(nameTemp2, nameTemp1) != NULL) {
			addFistList(searchList, temp1->item);
			found++;
		}
	}
	return found;
}

void passMenu() {
	FILE *f = fopen("Password.txt", "r");
	int quantityA = 20;
	int i = 0;
	Account *account = malloc(quantityA * sizeof(Account));
	char copy[512];
	fgets(copy, sizeof(copy), f);
	while (fgets(copy, sizeof(copy), f)) {
		char *temp = strtok(copy, "|");
		if (temp == NULL)
			continue;

		temp = deleteWhiteSpace(temp);
		strcpy((account + i)->employeeCode, temp);

		temp = strtok(NULL, "|");
		if (temp == NULL)
			continue;
		temp = deleteWhiteSpace(temp);

		strcpy((account + i)->password, temp);

		i++;
		if (i >= quantityA) {
			quantityA *= 2;
			account = realloc(account, quantityA * sizeof(Account));
		}
	}
	fclose(f);
	int check = 0;
	while (check == 0) {
		checkPass(&check, account, i);
	}
	return;
}

int checkListBuy(List *ds, Item item) {
	for (Node *p = ds->head; p != NULL; p = p->next) {
		if (p->item.barcode == item.barcode) {
			p->item.stockQuantity += item.stockQuantity;
			return 1;
		}
	}
	return 0;
}

int checkBarcode(List *ds, Item *item) {
	for (Node *p = ds->head; p != NULL; p = p->next) {
		if (p->item.barcode == item->barcode) {
			strcpy(item->name, p->item.name);
			item->price = p->item.price;
			return 1;
		}
	}
	printf("> Ma barcode khong dung! Vui long nhap lai!\n");
	return 0;
}

int checkBuy(char *text) {
	printf("> Bam xac nhan de %s\n", text);
	printf("1. Xac nhan (->)\n");
	printf("2. Huy bo (<-)\n");

	while (1) {
		int a = _getch();

		if (a == 0 || a == 224) {
			int b = _getch();
			if (b == 77) {
				printf("> Xac nhan thanh cong!\n");
				printf("Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n');
				return 1;
			} else if (b == 75) {
				printf("> Huy bo thanh cong!\n");
				printf("Nhan Enter de tiep tuc...\n");
				while (getchar() != '\n');
				return 0;
			}
		} else {
			printf("> Vui long nhap lai!\n");

		}
	}
}

void checkPass(int *i, Account *account, int number) {
	char employeeCode[10];
	char password[20];

	printf("> Ma nhan vien: 1111\n> Mat Khau: 1111\n");
	printf("> Dang nhap vao tai khoan nhan vien de thao tac:\n");

	printf("1. Nhap ma nhan vien: ");
	fgets(employeeCode, sizeof(employeeCode), stdin);
	employeeCode[strcspn(employeeCode, "\n")] = '\0';
	printf("2. Nhap mat khau: ");
	
	char c;
    int h=0;
    while(1){
        c = getch();
        if(c=='\r'||c=='\n'){
            password[h]='\0';
            break;
        } else if(c==8){
            if(h>0){
                h--;
                printf("\b \b");
            }
        } else {
            password[h]=c;
            h++;
            printf("*");
        }
    }
    
	password[strcspn(password, "\n")] = '\0';

	for (int k = 0; k < number; k++) {
		if (strcmp(employeeCode, (account + k)->employeeCode) == 0) {
			if (strcmp(password, (account + k)->password) == 0) {
				printf("\n> Da dung mat khau\n");
			while (getchar() != '\n');
				*i = 1;
				return;
			}
		}
	}
	printf("> Sai ma nhan vien hoac ten tai khoan. Vui long nhap lai!\n");
}

int sumBill(List bill) {
	int sum;
	for (Node *p = bill.head; p != NULL; p = p->next) {
		sum += (p->item.price * p->item.stockQuantity);
	}
	return sum;
}

void arrange(List *ds, int choose, int inc) {
	switch (choose) {
		case 1:
			// theo ten

			sortItem(ds, chooseName, inc, NULL, 0);

			break;
		case 2:
			// theo the loai

			sortItem(ds, chooseCategory, inc, NULL, 0);

			break;
		case 3:
			// theo gia tien

			sortItem(ds, NULL, inc, choosePrice, 0);

			break;
		case 4:
			// theo so luong

			sortItem(ds, NULL, inc, chooseStock, 0);

			break;

		case 5:
			// theo hang

			sortItem(ds, chooseSupplier, inc, NULL, 0);

			break;
		case 6:
			// theo hang san xuat
			sortItem(ds, NULL, inc, NULL, 1);
			break;
		case 7:
			// theo hang su dung
			sortItem(ds, NULL, inc, NULL, 2);
			break;
		case 8:
			// theo don vi

			sortItem(ds, chooseUnit, inc, NULL, 0);

			break;
	}
}

Item inputItem() {
	Item item;
	printf("1. Nhap name: ");
	fgets(item.name, sizeof(item.name), stdin);
	item.name[strcspn(item.name, "\n")] = '\0';

	while (1) {
		int tempBarcode;
		printf("2. Nhap ma Barcode: ");
		if (scanf("%d", &tempBarcode) == 1) {
			item.barcode = tempBarcode;
			while (getchar() != '\n');
			break;
		} else {
			printf("> Vui long nhap lai, LOI.\n");
			while (getchar() != '\n');
		}
	}
	printf("3. Nhap loai hang hoa: ");
	fgets(item.category, sizeof(item.category), stdin);
	item.category[strcspn(item.category, "\n")] = '\0';

	while (1) {
		printf("4. Nhap gia ca: ");
		if (scanf("%d", &item.price) == 1) {
			while (getchar() != '\n');
			break;
		} else {
			printf("> Vui long nhap lai, LOI.\n");
			while (getchar() != '\n');
		}
	}

	while (1) {
		printf("5. Nhap so luong trong kho: ");
		if (scanf("%d", &item.stockQuantity) == 1) {
			while (getchar() != '\n');
			break;
		} else {
			printf("> Vui long nhap lai, LOI.\n");
			while (getchar() != '\n');
		}
	}
	printf("6. Nhap mo ta ");
	fgets(item.description, sizeof(item.description), stdin);
	item.description[strcspn(item.description, "\n")] = '\0';

	printf("7. Nhap nha san xuat: ");
	fgets(item.supplier, sizeof(item.supplier), stdin);
	item.supplier[strcspn(item.supplier, "\n")] = '\0';

	while (1) {
		printf("8. Nhap HSX (dd-mm-yyyy): ");
		if (scanf("%d-%d-%d", &item.expiryData.HSX.day, &item.expiryData.HSX.month, &item.expiryData.HSX.year) == 3) {
			while (getchar() != '\n');
			break;
		} else {
			printf("> Vui long nhap lai, LOI.\n");
			while (getchar() != '\n');
		}
	}

	while (1) {
		printf("9. Nhap HSD (dd-mm-yyyy): ");
		if (scanf("%d-%d-%d", &item.expiryData.HSD.day, &item.expiryData.HSD.month, &item.expiryData.HSD.year) == 3) {
			while (getchar() != '\n');
			break;
		} else {
			printf("> Vui long nhap lai, LOI.\n");
			while (getchar() != '\n');
		}
	}
	printf("10. Nhap don vi tinh: ");
	fgets(item.unit, sizeof(item.unit), stdin);
	item.unit[strcspn(item.unit, "\n")] = '\0';
	
	
	return item;
}

void tabSet(Tab *tab) {
	tab->barcode = 10;
	tab->name = 30;
	tab->category = 10;
	tab->price = 7;
	tab->stockQuantity = 8;
	tab->description = 35;
	tab->supplier = 18;
	tab->HSX = 10;
	tab->HSD = 10;
	tab->unit = 6;
}

void showBill(List ds, Tab tab, int number) {
	printf("> Bill so %d:\n", number);
	printf("=========================================================================================\n");
	printf("| %-3s | %-*s | %-*s | %-*s | %-*s | %-*s |\n",
	       "No",
	       tab.name, "Name",
	       tab.barcode, "Barcode",
	       tab.stockQuantity, "Quantity",
	       tab.price, "Price",
	       tab.price, "Total");
	printf("=========================================================================================\n");

	int i = 1;
	int sumTotal = 0;

	for (Node *p = ds.head; p != NULL; p = p->next, i++) {
		int quantity;
		int total = p->item.price * p->item.stockQuantity;
		sumTotal += total;
		printf("| %-3d | %-*s | %-*d | %-*d | %-*d | %-*d |\n",
		       i,
		       tab.name, p->item.name,
		       tab.barcode, p->item.barcode,
		       tab.stockQuantity, p->item.stockQuantity,
		       tab.price, p->item.price,
		       tab.price, total);
	}

	printf("=========================================================================================\n");
	printf("%*s Total Amount: %d\n", tab.name + tab.barcode + tab.stockQuantity + tab.price * 2 + 1, "> ", sumTotal);
}

void tableBuy(List ds, Tab tab) {
	printf("========================================================================================\n");
	printf("| %-3s | %-*s | %-*s | %-*s | %-*s | %-*s |\n",
	       "No",
	       tab.name, "Name",
	       tab.barcode, "Barcode",
	       tab.stockQuantity, "Quantity",
	       tab.price, "Price",
	       tab.price, "Total");
	printf("=========================================================================================\n");

	int i = 1;
	int sumTotal = 0;

	for (Node *p = ds.head; p != NULL; p = p->next, i++) {
		int quantity;
		int total = p->item.price * p->item.stockQuantity;
		sumTotal += total;
		printf("| %-3d | %-*s | %-*d | %-*d | %-*d | %-*d |\n",
		       i,
		       tab.name, p->item.name,
		       tab.barcode, p->item.barcode,
		       tab.stockQuantity, p->item.stockQuantity,
		       tab.price, p->item.price,
		       tab.price, total);
	}

	printf("=========================================================================================\n");
	printf("%*s Total Amount: %d\n", tab.name + tab.barcode + tab.stockQuantity + tab.price * 2 + 1, "> ", sumTotal);
}

void tableHide(List ds, Tab tab) {
	printf("=====================================================================================================================================================================================\n");
	printf("| %-3s | %-30s | %-10s | %-10s | %-7s | %-8s | %-35s | %-18s | %-10s | %-10s | %-6s |\n",
	       "No", "Name", "Barcode", "Category", "Price", "StockQty", "Description", "Supplier", "HSX", "HSD", "Unit");
	printf("=====================================================================================================================================================================================\n");

	int i = 1;
	for (Node *p = ds.head; p != NULL; p = p->next, i++) {
		printf("| %-3d | %-*s | %-*d | %-*s | %-*d | %-*d | %-*s | %-*s | %02d-%02d-%04d | %02d-%02d-%04d | %-*s |\n",
		       i,
		       tab.name, p->item.name,
		       tab.barcode, p->item.barcode,
		       tab.category, p->item.category,
		       tab.price, p->item.price,
		       tab.stockQuantity, p->item.stockQuantity,
		       tab.description, p->item.description,
		       tab.supplier, p->item.supplier,
		       p->item.expiryData.HSX.day,
		       p->item.expiryData.HSX.month,
		       p->item.expiryData.HSX.year,
		       p->item.expiryData.HSD.day,
		       p->item.expiryData.HSD.month,
		       p->item.expiryData.HSD.year,
		       tab.unit, p->item.unit);
	}

	printf("=====================================================================================================================================================================================\n");
}

int checkWhileSpace(char a) {
	return a == ' ' || a == '\n';
}

char *deleteWhiteSpace(char *text) {
	char *endText;
	while (checkWhileSpace(*text)) {
		text++;
	}
	if (*text == '\0') {
		return text;
	}

	endText = text + strlen(text) - 1;
	while (checkWhileSpace(*endText)) {
		endText--;
	}

	*(endText + 1) = '\0';
	return text;
}

void saveDateHSX(Expiry *expiry, char *HSX) {
	sscanf(HSX, "%d-%d-%d", &expiry->HSX.day, &expiry->HSX.month, &expiry->HSX.year);
}

void saveDateHSD(Expiry *expiry, char *HSD) {
	sscanf(HSD, "%d-%d-%d", &expiry->HSD.day, &expiry->HSD.month, &expiry->HSD.year);
}

void addItem(List *ds, char *itemInfo) {
	Item newItem;
	char *info = strtok(itemInfo, "|");

	for (int i = 1; i < 11; i++) {

		info = deleteWhiteSpace(info);

		switch (i) {
			case 1:
				strcpy(newItem.name, info);
				break;
			case 2:

				newItem.barcode = atoi(info);
				break;
			case 3:

				strcpy(newItem.category, info);
				break;
			case 4:

				newItem.price = atoi(info);
				break;
			case 5:

				newItem.stockQuantity = atoi(info);
				break;
			case 6:

				strcpy(newItem.description, info);
				break;
			case 7:

				strcpy(newItem.supplier, info);
				break;
			case 8:

				saveDateHSX(&newItem.expiryData, info);
				break;
			case 9:

				saveDateHSD(&newItem.expiryData, info);
				break;
			case 10:

				strcpy(newItem.unit, info);
				break;
			default:
				break;
		}

		info = strtok(NULL, "|");
	}
	addFistList(ds, newItem);
}

void sortItem(List *ds, ChooseChar chooseChar, int inc, ChooseNumber chooseNumber, short HSXorHSD) {
	
	if (HSXorHSD == 1) {
		for (Node *temp1 = ds->head; temp1 != NULL; temp1 = temp1->next) {
			// duyet toi phan tu cuoi
			for (Node *temp2 = ds->head; temp2 != NULL; temp2 = temp2->next) {
				int day1 = temp1->item.expiryData.HSX.day;
				int day2 = temp2->item.expiryData.HSX.day;
				int month1 = temp1->item.expiryData.HSX.month;
				int month2 = temp2->item.expiryData.HSX.month;
				int year1 = temp1->item.expiryData.HSX.year;
				int year2 = temp2->item.expiryData.HSX.year;
				short a = 0;
				if (year1 > year2) {
					a = 1;
				} else if (year1 == year2) {
					if (month1 > month2) {
						a = 1;
					} else if (month1 == month2) {
						if (day1 > day2) {
							a = 1;
						} else if (day1 == day2) {
							a = 0;
						}
					}
				}
				if (a == 1 && inc == 0) {
					// temp1>temp2
					changeItem(ds, temp1, temp2);
				} else if (a == 1 && inc == 1) {
					changeItem(ds, temp2, temp1);
				}
			}
		}
		return;
	} else if (HSXorHSD == 2) {
		for (Node *temp1 = ds->head; temp1 != NULL; temp1 = temp1->next) {
			// duyet toi phan tu cuoi
			for (Node *temp2 = ds->head; temp2 != NULL; temp2 = temp2->next) {
				int day1 = temp1->item.expiryData.HSD.day;
				int day2 = temp2->item.expiryData.HSD.day;
				int month1 = temp1->item.expiryData.HSD.month;
				int month2 = temp2->item.expiryData.HSD.month;
				int year1 = temp1->item.expiryData.HSD.year;
				int year2 = temp2->item.expiryData.HSD.year;
				short a = 0;
				if (year1 > year2) {
					a = 1;
				} else if (year1 == year2) {
					if (month1 > month2) {
						a = 1;
					} else if (month1 == month2) {
						if (day1 > day2) {
							a = 1;
						} else if (day1 == day2) {
							a = 0;
						}
					}
				}
				if (a == 1 && inc == 0) {
					// temp1>temp2
					changeItem(ds, temp1, temp2);
				} else if (a == 1 && inc == 1) {
					changeItem(ds, temp2, temp1);
				}
			}
		}
		return;
	}

	if (chooseNumber == NULL) {
		// duyet toi phan tu cuoi
		for (Node *temp1 = ds->head; temp1 != NULL; temp1 = temp1->next) {
			// duyet toi phan tu cuoi
			for (Node *temp2 = ds->head; temp2 != NULL; temp2 = temp2->next) {
				char *char1 = chooseChar(&(temp1->item));
				char *char2 = chooseChar(&(temp2->item));

				// s nho hon t
				if (strcmp(char1, char2) < 0 && inc == 1) {
					changeItem(ds, temp1, temp2);
				} else if (strcmp(char1, char2) > 0 && inc == 0) {
					changeItem(ds, temp1, temp2);
				}
			}
		}
	} else {
		// duyet toi phan tu cuoi
		for (Node *temp1 = ds->head; temp1 != NULL; temp1 = temp1->next) {
			// duyet toi phan tu cuoi
			for (Node *temp2 = ds->head; temp2 != NULL; temp2 = temp2->next) {
				int number1 = chooseNumber(&(temp1->item));
				int number2 = chooseNumber(&(temp2->item));

				// s nho hon t
				if (number1 < number2 && inc == 1) {
					changeItem(ds, temp1, temp2);
				} else if (number1 > number2 && inc == 0) {
					changeItem(ds, temp1, temp2);
				}
			}
		}
	}
}

int choosePrice(Item *item) {
	return item->price;
}
int chooseStock(Item *item) {
	return item->stockQuantity;
}

char *chooseName(Item *item) {
	return item->name;
}

char *chooseCategory(Item *item) {
	return item->category;
}
char *chooseSupplier(Item *item) {
	return item->supplier;
}
char *chooseUnit(Item *item) {
	return item->unit;
}
void ds(List *ds) {
	ds->head = NULL;
	ds->tail = NULL;
}

Node *createNode(Item item) {
	Node *newNode = (Node *)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("Khong cap phat duoc bo nho");
		return NULL;
	}
	newNode->item = item;
	newNode->next = NULL;
	return newNode;
}

void addFistList(List *ds, Item item) {
	Node *newNode = createNode(item);
	if (newNode == NULL) {
		printf("Loi vi newNode== NULL");
		return;
	}
	if (ds->head == NULL) {
		ds->head = newNode;
		ds->tail = newNode;
	} else {
		newNode->next = ds->head;
		ds->head = newNode;
	}
}

void changeItem(List *ds, Node *temp1, Node *temp2) {
	if (temp1 == temp2) {
		return;
	}

	Item tempItem;
	tempItem = temp1->item;
	temp1->item = temp2->item;
	temp2->item = tempItem;
}
