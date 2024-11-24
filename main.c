#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

int slots[5][5] = {
    {3, 5, 4, 2, 6},
    {4, 3, 5, 2, 1},
    {6, 4, 3, 2, 5},
    {2, 3, 5, 6, 4},
    {5, 6, 4, 3, 2}};

typedef struct Patient {
    int id;
    char name[50];
    int age;
    char gender[10];
    char severity[20];
    char disease[50];
    struct Patient* next;
} Patient;
Patient* patientHead = NULL;

typedef struct Doctor {
    int id;
    char name[50];
    char specialty[50];
    int availableSlots;
    struct Doctor* next;
} Doctor;
Doctor* doctorHead = NULL;

typedef struct Appointment {
    char patientNam[50];
    int doctorID;
    struct Appointment* next;
} Appointment;

Appointment* appointmentFront = NULL;  // Queue front
Appointment* appointmentRear = NULL;   // Queue rear

Patient* searchPatientByID(int id) {
    Patient* temp = patientHead;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;  // Return NULL if the patient is not found
}

Doctor* searchDoctorByID(int id) {
    Doctor* temp = doctorHead;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;  // Doctor not found
}

void menu();
void login();
void admin();
void initializeDoctors();
void returnlanding();
void slowTxt();
void addPatient();
void loadPatientsFromFile();
void savePatientsToFile();
void displayPatients();
void firstinterface();
void inputPatientData();
void updatePatient();
void deletePatient();
void deletePatientByID();
void makeAppointment();
void browseDoctorsBySpecialty();

int main() {
    system("cls");
    initializeDoctors();
    loadPatientsFromFile();
    firstinterface();
    login();
    system("cls");
    return 0;
}

void firstinterface() {
    char ab[] = "||||||||||||||||||||||||||||||||||||||||||||";
    char ar[] = "||||||||   Welcome to our project   ||||||||";
    printf("\n\n\t%s\n", ab);
    printf("\t");
    for (int i = 0; i < sizeof(ar) - 1; i++) {
        Sleep(25);
        printf("\033[1m%c", ar[i]);
    }
    printf("\033[0m\n");
    printf("\t%s\n", ab);
    printf("\n\n");
}

void deletePatientByID(int id) {
    if (patientHead == NULL) {
        printf("No patients to delete.\n");
        return;
    }

    Patient *temp = patientHead, *prev = NULL;

    // If the head is the patient to delete
    if (temp->id == id) {
        patientHead = temp->next;  // Move the head
        free(temp);                // Free the old head
        printf("Patient with ID %d deleted successfully.\n", id);
        savePatientsToFile();  // Save updated patient list to file
        return;
    }

    // Search for the patient to delete
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    // If the patient was not found
    if (temp == NULL) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    // Remove the patient from the linked list
    prev->next = temp->next;
    free(temp);
    printf("Patient with ID %d deleted successfully.\n", id);

    // Save updated list to the file
    savePatientsToFile();
}

void slowTxt(char* str) {
    system("CLS");
    printf("\n\n");
    printf("\t=============================================\n");
    Sleep(20);
    printf("\n\t        Hospital Management System\n\n");
    Sleep(20);
    printf("\t=============================================\n");
    Sleep(20);
    printf("\n\n\t");
    int x = strlen(str);
    for (int i = 0; i < x; i++) {
        printf("%c", str[i]);
        Sleep(20);
    }
    printf("\n\n");
}

void loadPatientsFromFile() {
    FILE* file = fopen("patients.dat", "r");
    if (file == NULL) {
        printf("\tNo patient data found. Starting fresh.\n");
        return;
    }

    Patient* temp = NULL;

    while (1) {
        Patient* newPatient = (Patient*)malloc(sizeof(Patient));
        if (newPatient == NULL) {
            printf("\tMemory allocation failed.\n");
            break;
        }

        // Read patient data
        if (fscanf(file, "%d,%49[^,],%d,%9[^,],%19[^,],%49[^\n]\n",
                   &newPatient->id, newPatient->name, &newPatient->age,
                   newPatient->gender, newPatient->severity, newPatient->disease) == 6) {
            newPatient->next = NULL;
            if (patientHead == NULL) {
                patientHead = newPatient;
                temp = patientHead;
            } else {
                temp->next = newPatient;
                temp = newPatient;
            }
        } else {
            free(newPatient);  // Free allocated memory if the line is invalid
            break;
        }
    }

    fclose(file);
    // printf("\tPatient data loaded successfully.\n");
}

void savePatientsToFile() {
    FILE* file = fopen("patients.dat", "w");
    if (file == NULL) {
        printf("Error: Unable to save patient data.\n");
        return;
    }

    Patient* temp = patientHead;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%d,%s,%s,%s\n", temp->id, temp->name, temp->age,
                temp->gender, temp->severity, temp->disease);
        temp = temp->next;
    }

    fclose(file);
    printf("\tPatient data saved successfully.\n");
}

void addPatient(int id, const char* name, int age, const char* gender, const char* disease, const char* severity) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    newPatient->id = id;
    strcpy(newPatient->name, name);
    newPatient->age = age;
    strcpy(newPatient->gender, gender);
    strcpy(newPatient->disease, disease);
    strcpy(newPatient->severity, severity);
    newPatient->next = NULL;

    if (patientHead == NULL) {
        patientHead = newPatient;
    } else {
        Patient* temp = patientHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPatient;
    }
}

void displayPatients() {
    Patient* temp = patientHead;
    char ar[] = "Patient List:";
    printf("\t");
    for (int i = 0; i < sizeof(ar) - 1; i++) {
        Sleep(25);
        printf("%c", ar[i]);
    }
    printf("\n");
    while (temp != NULL) {
        // printf("\t\tID: %d, Name: %s, Age: %d, Gender: %s, Disease: %s\n", temp->id, temp->name, temp->age, temp->gender, temp->disease);
        printf("\t\tID      : %d\n\t\tName    : %s\n\t\tAge     : %d\n\t\tGender  : %s\n\t\tDisease : %s\n", temp->id, temp->name, temp->age, temp->gender, temp->disease);
        temp = temp->next;
        printf("\n");
    }
    printf("\n");
}

void inputPatientData() {
    int id, age;
    char name[50], gender[10], disease[50], severity[20];
    char s[] = "You wanted to add a new Patient. \n\tPlease enter his/her detailed information";
    slowTxt(s);

    printf("\t=============================================\n");
    printf("\n");
    printf("\tEnter Patient ID: ");
    scanf("%d", &id);

    printf("\tEnter Patient Name: ");
    scanf(" %[^\n]s", name);  // Using " %[^\n]s" to read a full line with spaces

    printf("\tEnter Patient Age: ");
    scanf("%d", &age);

    printf("\tEnter Patient Gender: ");
    scanf(" %[^\n]s", gender);

    printf("\tEnter Patient Disease: ");
    scanf(" %[^\n]s", disease);

    printf("\tEnter Severity (e.g., Mild, Moderate, Severe): ");
    scanf(" %[^\n]s", severity);

    addPatient(id, name, age, gender, disease, severity);  // Call the updated addPatient function
    printf("\tPatient details added successfully!\n\n");
    printf("\t=============================================\n");
    savePatientsToFile();
}

void updatePatient(int id) {
    Patient* patient = searchPatientByID(id);
    if (patient == NULL) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    printf("\tEnter New Patient Name: ");
    scanf(" %[^\n]s", patient->name);
    printf("\tEnter New Patient Age: ");
    scanf("%d", &patient->age);
    printf("\tEnter New Patient Gender: ");
    scanf(" %[^\n]s", patient->gender);
    printf("\tEnter New Patient Disease: ");
    scanf(" %[^\n]s", patient->disease);
    printf("\tEnter Severity (e.g., Mild, Moderate, Severe): ");
    scanf(" %[^\n]s", patient->severity);
    savePatientsToFile();
    printf("\tPatient information updated successfully!\n");
}

void deletePatient(int id) {
    Patient *temp = patientHead, *prev = NULL;

    // Check if the head node contains the patient
    if (temp != NULL && temp->id == id) {
        patientHead = temp->next;  // Move head to the next node
        free(temp);                // Free the old head
        printf("\tPatient with ID %d deleted successfully.\n", id);
        return;
    }

    // Traverse the list to find the patient
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    // If patient not found
    if (temp == NULL) {
        printf("\tPatient with ID %d not found.\n", id);
        return;
    }

    // Remove the patient from the linked list
    prev->next = temp->next;
    free(temp);
    printf("\tPatient with ID %d deleted successfully.\n", id);
}

void initializeDoctors() {
    doctorHead = NULL;  // Initialize the head of the linked list

    // Specialties
    char specialties[5][50] = {
        "Cardiology", "Orthopedics", "Dermatology", "Pediatrics", "Neurology"};

    // Doctor names for each specialty
    char doctorNames[5][5][100] = {
        {"Assoc. Prof. Dr. Bijoy Dutta", "Prof. Dr. Md. Sahabuddin Khan", "Prof. Dr. Toufiqur Rahman Faruque", "Dr. AKS Zahid Mahmud Khan", "Prof. Dr. Ashok Kumar Dutta"},
        {"Asst. Prof. Dr. Md. Nazmul Huda", "Dr. Md. Mizanur Rahman", "Dr. M A Mamun", "Dr. K M Shorfuddin Ashik", "Prof. Dr. Md. Kamrul Ahsan"},
        {"Dr. Asif Imran Siddiqui", "Dr. Farzana Rahman Shathi", "Prof. Dr. M.N. Huda", "Prof. Lt. Col. Dr. Md. Abdul Wahab", "Prof. Dr. M. U. Kabir Chowdhury"},
        {"Dr. Mithun Sarker", "Dr. Chowdhury Md. Niazuzzaman", "Dr. Hasan Mahmud Abdullah", "Dr. Md. Zahidul Islam", "Dr. Md. Waliur Rahman"},
        {"Dr. Shamim Rashid", "Dr. Md. Shuktarul Islam (Tamim)", "Dr. Mohiuddin Ahmed", "Dr. Rakib Hasan Mohammad", "Prof. Dr. Subash Kanti Dey"}};

    // Slots available for each doctor
    int slots[5][5] = {
        {3, 5, 4, 2, 6},
        {4, 3, 5, 2, 1},
        {6, 4, 3, 2, 5},
        {2, 3, 5, 6, 4},
        {5, 6, 4, 3, 2}};

    // Populate the doctor list
    Doctor* temp = NULL;

    for (int i = 0; i < 5; i++) {      // Loop through specialties
        for (int j = 0; j < 5; j++) {  // Loop through doctors in each specialty
            Doctor* newDoctor = (Doctor*)malloc(sizeof(Doctor));
            newDoctor->id = i * 5 + j + 1;  // Generate unique IDs
            strcpy(newDoctor->name, doctorNames[i][j]);
            strcpy(newDoctor->specialty, specialties[i]);
            newDoctor->availableSlots = slots[i][j];
            newDoctor->next = NULL;

            if (doctorHead == NULL) {
                doctorHead = newDoctor;
                temp = doctorHead;
            } else {
                temp->next = newDoctor;
                temp = newDoctor;
            }
        }
    }
}

void makeAppointment(char patientName[], int doctorID) {
    Doctor* doctor = searchDoctorByID(doctorID);

    if (doctor == NULL) {
        printf("Doctor with ID %d not found.\n", doctorID);
        return;
    }

    if (doctor->availableSlots <= 0) {
        printf("No slots available for Doctor ID %d (%s).\n", doctorID, doctor->name);
        return;
    }

    // Reduce the available slots and schedule the appointment
    doctor->availableSlots--;

    printf("Appointment confirmed for Patient: %s with Doctor ID %d (%s).\n", patientName, doctorID, doctor->name);
    // printf("Your slot number is %d.\n", doctor->availableSlots + 1);
    // printf("%d %d\n",(doctorID - 1) / 5,(doctorID - 1) % 5);
    // printf("%d\n", slots[(doctorID - 1) / 5][(doctorID - 1) % 5]);
    printf("Your slot number is %d.\n", slots[(doctorID - 1) / 5][(doctorID - 1) % 5] - doctor->availableSlots);

    returnlanding();
}

void browseDoctorsBySpecialty() {
    Doctor* temp = doctorHead;
    char specialties[10][50];  // Assuming a maximum of 10 specialties
    int specialtyCount = 0;

    // Step 1: Extract unique specialties
    while (temp != NULL) {
        int found = 0;
        for (int i = 0; i < specialtyCount; i++) {
            if (strcmp(specialties[i], temp->specialty) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(specialties[specialtyCount], temp->specialty);
            specialtyCount++;
        }
        temp = temp->next;
    }

    // Step 2: Display specialties
    printf("\t=============================================\n\n");
    printf("\tAvailable Specialties:\n");
    for (int i = 0; i < specialtyCount; i++) {
        printf("\t%d. %s\n", i + 1, specialties[i]);
    }
    printf("\n\t=============================================\n\n");

    printf("\tEnter the number of the specialty you want to browse: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > specialtyCount) {
        printf("\n\tInvalid choice! Returning to Patient Menu.\n");
        return;
    }

    // Step 3: Show doctors in the selected specialty
    char selectedSpecialty[50];
    strcpy(selectedSpecialty, specialties[choice - 1]);
    temp = doctorHead;

    printf("\t=============================================\n\n");
    printf("\tDoctors in Specialty: %s\n", selectedSpecialty);
    int doctorFound = 0;
    while (temp != NULL) {
        if (strcmp(temp->specialty, selectedSpecialty) == 0) {
            printf("\t\033[1mDoctor ID: %d\033[0m \n\t\tName            : %s\n\t\tSlots Available : %d\n\n",
                   temp->id, temp->name, temp->availableSlots);
            doctorFound = 1;
        }
        temp = temp->next;
    }
    printf("\n\t=============================================\n\n");

    if (!doctorFound) {
        printf("\n\tNo doctors available for this specialty.\n");
        return;
    }

    // Step 4: Allow patient to make an appointment
    char patientName[50];
    int doctorID;

    printf("\nEnter your name: ");
    scanf(" %[^\n]s", patientName);

    printf("Enter the Doctor ID to make an appointment: ");
    scanf("%d", &doctorID);

    makeAppointment(patientName, doctorID);
}

void menu() {
    int choice, id;
    printf("\n\n");
    printf("\t=============================================\n\n");
    printf("\t[1] Add New Patient\n");
    printf("\t[2] Display Patients\n");
    printf("\t[3] Search Patient by ID\n");
    printf("\t[4] Update Patient Information\n");
    printf("\t[5] Delete Patient by ID\n");
    printf("\t[6] Return to Home\n");
    printf("\t[7] Exit\n\n");
    printf("\t=============================================\n\n");
    printf("\tEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            inputPatientData();
            break;
        case 2:
            displayPatients();
            break;
        case 3:
            printf("\tEnter Patient ID to search: ");
            scanf("%d", &id);
            Patient* patient = searchPatientByID(id);
            if (patient) {
                printf("\tPatient Found: \n\t\tID      : %d\n\t\tName    : %s\n\t\tAge     : %d\n\t\tGender  : %s\n\t\tDisease : %s\n", patient->id, patient->name, patient->age, patient->gender, patient->disease);
            } else {
                printf("\tPatient not found.\n");
            }
            break;
        case 4:
            printf("\tEnter Patient ID to update: ");
            scanf("%d", &id);
            updatePatient(id);
            break;
        case 5:
            printf("\tEnter Patient ID to delete: ");
            scanf("%d", &id);
            deletePatientByID(id);
            break;
        case 6:
            login();
            break;
        case 7:
            printf("\tExiting...\n");
            return;
        default:
            printf("\tInvalid choice! Please try again.\n");
    }
    returnlanding();
}

void login() {
    int j;
    printf("\t=============================================\n\n");
    printf("\n\t\t\t1. Admin Login\n\n");
    printf("\t\t\t2. For Patient\n\n");

    int x;
    printf("\tEnter Your Choice : ");
    scanf("%d", &x);
    if (x == 1) {
        int pass = 1234, pas;

        printf(" \n                       Username       : Admin");

        printf(" \n                       ENTER PASSWORD : ");

        scanf("%d", &pas);

        if (pass == pas) {
            printf("  \n\n\n");
            char str[] = "       WELCOME !!!! LOGIN IS SUCCESSFUL";
            int x = strlen(str);
            for (int i = 0; i < x; i++) {
                printf("%c", str[i]);
                Sleep(20);
            }
            Sleep(1000);
            system("cls");
            // system("color 8f");
            printf("\n\n\n\n\n\n");
            printf("                                                           \n");
            printf("                                                           \n");
            printf("                      \t  Please Wait...\n\n\n\n\n");
            printf("                     ______________ _____________               \n");
            printf("                    /                             \\          \n");
            printf("                   |     Loading.............      |           \n");
            printf("\t\t   |\t");
            for (j = 0; j < 24; j++) {
                printf("%c", 219);
                Sleep(50);
            }
            printf("   |\n                    \\______________ ______________/           \n");
            printf("                                                           \n");
            printf("\n\n\n");
            // system("pause");
            Sleep(1000);
            system("cls");
            system("color 0f");
            menu();
        } else {
            printf("Invalid Password !\n\n");
            Sleep(40);
            login();
        }
    }

    if (x == 2) {
        printf("\n\n\t\t\t1. Our Services.\n");
        printf("\t\t\t2. Make Appointment.\n\n");
        int a;
        printf("\tEnter your Choice: ");
        scanf("%d", &a);
        if (a == 1) {
        } else if (a == 2) {
            browseDoctorsBySpecialty();
        }
    }
}

void returnlanding() {
    printf("\n\tTo return Home[H]\n\tTo return to Main Menu[M]\n\tTo Close the Programme[0]\n\tEnter your choice: ");
    char x;
    scanf(" %c", &x);
    if (x == '0') {
        return;
    } else if (x == 'M' || x == 'm') {
        menu();
        return;
    } else {
        login();
        return;
    }
}