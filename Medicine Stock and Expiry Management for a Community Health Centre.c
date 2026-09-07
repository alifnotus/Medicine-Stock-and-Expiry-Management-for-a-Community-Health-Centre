#include <stdio.h> 

#include <string.h> 

  

#define MEDICINE_COUNT 6 

#define CODE_LENGTH 10 

#define NAME_LENGTH 30 

#define SHORTAGE_DAYS 7 

#define EXPIRY_DAYS 30 

  

enum { 

    AVAILABLE_STOCK, 

    DAILY_REQUIREMENT, 

    MINIMUM_STOCK, 

    DAYS_TO_EXPIRY, 

    ESSENTIALITY 

}; 

  

char medicineCode[MEDICINE_COUNT][CODE_LENGTH] = { 

    "MED01", "MED02", "MED03", "MED04", "MED05", "MED06" 

}; 

  

char medicineName[MEDICINE_COUNT][NAME_LENGTH] = { 

    "Oral Saline", "Paracetamol", "Insulin", 

    "Amoxicillin", "Antacid", "Cetirizine" 

}; 

  

/* Each row contains: 

   [0] available stock 

   [1] average daily requirement 

   [2] minimum stock level 

   [3] days remaining to expiry 

   [4] essentiality level 

*/ 

int medicineData[MEDICINE_COUNT][5] = { 

    {120, 35, 80, 45, 3}, 

    {300, 40, 100, 120, 2}, 

    {60, 12, 50, 30, 3}, 

    {200, 25, 80, 20, 3}, 

    {250, 18, 60, 15, 1}, 

    {180, 15, 50, 90, 1} 

}; 

  

double calculateCoverage(int index) 

{ 

    if (medicineData[index][DAILY_REQUIREMENT] <= 0) 

        return 0.0; 

  

    return (double)medicineData[index][AVAILABLE_STOCK] / 

           medicineData[index][DAILY_REQUIREMENT]; 

} 

  

int shortageRisk(int index) 

{ 

    double coverage = calculateCoverage(index); 

  

    if (medicineData[index][AVAILABLE_STOCK] < 

            medicineData[index][MINIMUM_STOCK] || 

        coverage < SHORTAGE_DAYS) 

        return 1; 

  

    return 0; 

} 

  

int expiryRisk(int index) 

{ 

    double coverage = calculateCoverage(index); 

  

    if (medicineData[index][DAYS_TO_EXPIRY] <= EXPIRY_DAYS || 

        medicineData[index][DAYS_TO_EXPIRY] <= coverage) 

        return 1; 

  

    return 0; 

} 

  

/* 

    Condition severity: 

    1 = Critical Condition 

    2 = Urgent Reorder 

    3 = Reorder Required 

    4 = Expiry Attention 

    5 = Sufficient Stock 

*/ 

int conditionSeverity(int index) 

{ 

    int shortage = shortageRisk(index); 

    int expiry = expiryRisk(index); 

  

    if (shortage && expiry) 

        return 1; 

  

    if (shortage) 

    { 

        if (medicineData[index][ESSENTIALITY] == 3) 

            return 2; 

  

        return 3; 

    } 

  

    if (expiry) 

        return 4; 

  

    return 5; 

} 

  

const char *getCondition(int index) 

{ 

    int severity = conditionSeverity(index); 

  

    if (severity == 1) 

        return "Critical Condition"; 

    else if (severity == 2) 

        return "Urgent Reorder"; 

    else if (severity == 3) 

        return "Reorder Required"; 

    else if (severity == 4) 

        return "Expiry Attention"; 

  

    return "Sufficient Stock"; 

} 

  

/* 

    Returns 1 when the first medicine should appear before 

    the second medicine in the final priority order. 

*/ 

int comparePriority(int first, int second) 

{ 

    int firstSeverity = conditionSeverity(first); 

    int secondSeverity = conditionSeverity(second); 

  

    /* 1. Higher essentiality level */ 

    if (medicineData[first][ESSENTIALITY] != 

        medicineData[second][ESSENTIALITY]) 

    { 

        return medicineData[first][ESSENTIALITY] > 

               medicineData[second][ESSENTIALITY]; 

    } 

  

    /* 2. More severe condition */ 

    if (firstSeverity != secondSeverity) 

        return firstSeverity < secondSeverity; 

  

    /* 3. Lower stock coverage */ 

    if (calculateCoverage(first) != calculateCoverage(second)) 

        return calculateCoverage(first) < calculateCoverage(second); 

  

    /* 4. Earlier expiry */ 

    if (medicineData[first][DAYS_TO_EXPIRY] != 

        medicineData[second][DAYS_TO_EXPIRY]) 

    { 

        return medicineData[first][DAYS_TO_EXPIRY] < 

               medicineData[second][DAYS_TO_EXPIRY]; 

    } 

  

    /* 5. Original order */ 

    return first < second; 

} 

  

void sortMedicineIndexes(int order[]) 

{ 

    int i, j, temp; 

  

    for (i = 0; i < MEDICINE_COUNT - 1; i++) 

    { 

        for (j = 0; j < MEDICINE_COUNT - 1 - i; j++) 

        { 

            if (!comparePriority(order[j], order[j + 1])) 

            { 

                temp = order[j]; 

                order[j] = order[j + 1]; 

                order[j + 1] = temp; 

            } 

        } 

    } 

} 

  

int searchMedicine(const char searchCode[]) 

{ 

    int i; 

  

    for (i = 0; i < MEDICINE_COUNT; i++) 

    { 

        if (strcmp(medicineCode[i], searchCode) == 0) 

            return i; 

    } 

  

    return -1; 

} 

  

void displayMedicine(int index) 

{ 

    printf("\nMedicine Code        : %s\n", medicineCode[index]); 

    printf("Medicine Name        : %s\n", medicineName[index]); 

    printf("Available Stock      : %d\n", 

           medicineData[index][AVAILABLE_STOCK]); 

    printf("Daily Requirement    : %d\n", 

           medicineData[index][DAILY_REQUIREMENT]); 

    printf("Minimum Stock        : %d\n", 

           medicineData[index][MINIMUM_STOCK]); 

    printf("Days to Expiry       : %d\n", 

           medicineData[index][DAYS_TO_EXPIRY]); 

    printf("Essentiality Level   : %d\n", 

           medicineData[index][ESSENTIALITY]); 

    printf("Stock Coverage       : %.2f days\n", 

           calculateCoverage(index)); 

    printf("Shortage Risk        : %s\n", 

           shortageRisk(index) ? "Yes" : "No"); 

    printf("Expiry Risk          : %s\n", 

           expiryRisk(index) ? "Yes" : "No"); 

    printf("Stock Condition      : %s\n", 

           getCondition(index)); 

} 

  

int receiveStock(int *stock, int quantity) 

{ 

    if (quantity <= 0) 

        return 0; 

  

    *stock = *stock + quantity; 

    return 1; 

} 

  

int issueStock(int *stock, int quantity) 

{ 

    if (quantity <= 0 || quantity > *stock) 

        return 0; 

  

    *stock = *stock - quantity; 

    return 1; 

} 

  

void demonstrateSearch(const char searchCode[]) 

{ 

    int index = searchMedicine(searchCode); 

  

    printf("\n================ STOCK SEARCH ================\n"); 

    printf("Search code: %s\n", searchCode); 

  

    if (index == -1) 

        printf("Medicine not found.\n"); 

    else 

        displayMedicine(index); 

} 

  

void demonstrateUpdates(void) 

{ 

    int index; 

  

    printf("\n================ STOCK UPDATE ================\n"); 

  

    index = searchMedicine("MED02"); 

  

    if (index != -1) 

    { 

        if (receiveStock(&medicineData[index][AVAILABLE_STOCK], 50)) 

        { 

            printf("Received 50 units of %s.\n", medicineName[index]); 

            printf("New stock: %d\n", 

                   medicineData[index][AVAILABLE_STOCK]); 

            printf("Updated condition: %s\n", getCondition(index)); 

        } 

    } 

  

    index = searchMedicine("MED05"); 

  

    if (index != -1) 

    { 

        if (issueStock(&medicineData[index][AVAILABLE_STOCK], 40)) 

        { 

            printf("Issued 40 units of %s.\n", medicineName[index]); 

            printf("New stock: %d\n", 

                   medicineData[index][AVAILABLE_STOCK]); 

            printf("Updated condition: %s\n", getCondition(index)); 

        } 

    } 

} 

  

void displayFinalReport(int order[]) 

{ 

    int i, index; 

    int reorderCount = 0; 

    int urgentCount = 0; 

    int expiryCount = 0; 

  

    printf("\n\n================ FINAL STOCK REPORT ================\n"); 

    printf("%-6s %-15s %-7s %-10s %-7s %-5s %-20s %-8s\n", 

           "Code", "Medicine", "Stock", "Coverage", 

           "Expiry", "Ess.", "Condition", "Priority"); 

  

    for (i = 0; i < MEDICINE_COUNT; i++) 

    { 

        index = order[i]; 

  

        printf("%-6s %-15s %-7d %-10.2f %-7d %-5d %-20s %-8d\n", 

               medicineCode[index], 

               medicineName[index], 

               medicineData[index][AVAILABLE_STOCK], 

               calculateCoverage(index), 

               medicineData[index][DAYS_TO_EXPIRY], 

               medicineData[index][ESSENTIALITY], 

               getCondition(index), 

               i + 1); 

  

        if (shortageRisk(index)) 

            reorderCount++; 

  

        if (conditionSeverity(index) == 1 || 

            conditionSeverity(index) == 2) 

            urgentCount++; 

  

        if (expiryRisk(index)) 

            expiryCount++; 

    } 

  

    printf("\nSummary\n"); 

    printf("Total medicines analysed       : %d\n", MEDICINE_COUNT); 

    printf("Number requiring reorder       : %d\n", reorderCount); 

    printf("Number requiring urgent action : %d\n", urgentCount); 

    printf("Number with expiry concern     : %d\n", expiryCount); 

    printf("Highest management priority    : %s (%s)\n", 

           medicineName[order[0]], medicineCode[order[0]]); 

} 

  

int main(void) 

{ 

    int order[MEDICINE_COUNT]; 

    int i; 

  

    for (i = 0; i < MEDICINE_COUNT; i++) 

        order[i] = i; 

  

    printf("MEDICINE STOCK AND EXPIRY MANAGEMENT\n"); 

    printf("Community Health Centre\n"); 

  

    printf("\n================ INITIAL ANALYSIS ================\n"); 

  

    for (i = 0; i < MEDICINE_COUNT; i++) 

        displayMedicine(i); 

  

    demonstrateSearch("MED03"); 

    demonstrateSearch("MED99"); 

  

    demonstrateUpdates(); 

  

    sortMedicineIndexes(order); 

  

    displayFinalReport(order); 

  

    return 0; 

}
