#include "inverted_index.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (!strstr(argv[1], ".txt")) {
            printf("Error: Only .txt files are allowed. Skipping %s\n", argv[1]);
            return 1;
        }
    }

    int choice;
    char filename[FILE_NAME_SIZE];

    while (1) {
        printf("\n===== Inverted Index Menu =====\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Save Database\n");
        printf("4. Search Word\n");
        printf("5. Update Database\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the filename: ");
                scanf("%s", filename);
                if (is_valid_file(filename))
                    create_database(filename);
                break;

            case 2:
                display_database();
                break;

            case 3:
                save_database();
                break;

            case 4:
                search_database();
                break;

            case 5:
                update_database();
                break;

            case 6:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }
}
