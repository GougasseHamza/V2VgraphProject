#include "Vehicule.h"
#include "Simulation.h"
#include "graphalgos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_BOLD    "\x1b[1m"

void printHeader() {
    printf("\n");
    printf(COLOR_CYAN COLOR_BOLD);
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║           V 2 V   S I M U L A T I O N                     ║\n");
    printf("║         Vehicle-to-Vehicle Network Analyzer               ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
}

void printBox(const char* title, const char* content) {
    printf("\n" COLOR_GREEN "┌─ %s " COLOR_RESET "\n", title);
    printf("│ %s\n", content);
    printf(COLOR_GREEN "└────────────────────────────────────────────────────────────\n" COLOR_RESET);
}

void printMenu() {
    printf("\n" COLOR_YELLOW COLOR_BOLD);
    printf("┌──────────────────────────────────────────────┐\n");
    printf("│              ANALYSIS  MENU                  │\n");
    printf("├──────────────────────────────────────────────┤\n");
    printf(COLOR_RESET COLOR_YELLOW);
    printf("│  [1] " COLOR_RESET "Run Complete Simulation          " COLOR_YELLOW "│\n");
    printf("│  [2] " COLOR_RESET "Run to Specific Time             " COLOR_YELLOW "│\n");
    printf("│  [3] " COLOR_RESET "Analyze Clusters (DSU)           " COLOR_YELLOW "│\n");
    printf("│  [4] " COLOR_RESET "Compute Backbone MST             " COLOR_YELLOW "│\n");
    printf("│  [5] " COLOR_RESET "Identify Critical Hubs           " COLOR_YELLOW "│\n");
    printf("│  [6] " COLOR_RESET "Find Fastest Path                " COLOR_YELLOW "│\n");
    printf("│  [7] " COLOR_RESET "Show Current Status              " COLOR_YELLOW "│\n");
    printf("│  [0] " COLOR_RESET "Exit Simulation                  " COLOR_YELLOW "│\n");
    printf("└──────────────────────────────────────────────┘\n" COLOR_RESET);
    printf("\nChoice: ");
}

void printStatus(int timeStep, int infectedNumber) {
    printf("\n" COLOR_BLUE COLOR_BOLD);
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    CURRENT STATUS                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf(COLOR_RESET);
    printf("║  " COLOR_CYAN "⏱  Time Step:      " COLOR_RESET "%-30d  ║\n", timeStep);
    printf("║  " COLOR_RED "🦠 Infected Count: " COLOR_RESET "%-30d  ║\n", infectedNumber);
    printf("║  " COLOR_GREEN "🚗 Total Vehicles: " COLOR_RESET "%-30d  ║\n", MAX_VEHICULES);
    printf("║  " COLOR_YELLOW "📡 Danger Radius:  " COLOR_RESET "%-30d  ║\n", DANGER_RADIUS);
    printf(COLOR_BLUE);
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf(COLOR_RESET);
}

void printProgressBar(int current, int total, int infected) {
    int barWidth = 40;
    float progress = (float)current / total;
    int pos = barWidth * progress;

    printf("\r" COLOR_CYAN "Progress: " COLOR_RESET "[");
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) printf(COLOR_GREEN "█" COLOR_RESET);
        else if (i == pos) printf(COLOR_GREEN "▶" COLOR_RESET);
        else printf(COLOR_BLUE "░" COLOR_RESET);
    }
    printf("] " COLOR_YELLOW "%3d%%" COLOR_RESET " | T=%d/%d | " COLOR_RED "🦠 %d" COLOR_RESET,
           (int)(progress * 100), current, total, infected);
    fflush(stdout);
}

void runCompleteSimulation(int *timeStep, int *infectedNumber) {
    printf("\n" COLOR_MAGENTA COLOR_BOLD "▶ Running complete simulation (60 time steps)..." COLOR_RESET "\n\n");

    int maxTime = 60;
    int lastInfected = 0;
    int newInfections = 0;

    while(*timeStep <= maxTime) {
        lastInfected = *infectedNumber;
        runStep(1.0f);
        infectVehicules(*timeStep, infectedNumber);

        newInfections = *infectedNumber - lastInfected;
        if(newInfections > 0 && *timeStep % 5 == 0) {
            printf("\n" COLOR_YELLOW "⚡ T=%d: +%d new infections" COLOR_RESET, *timeStep, newInfections);
        }

        if(*timeStep % 2 == 0 || *timeStep == maxTime) {
            printProgressBar(*timeStep, maxTime, *infectedNumber);
        }

        (*timeStep)++;
    }

    printf("\n\n" COLOR_GREEN COLOR_BOLD "✓ Simulation complete!" COLOR_RESET "\n");
    printf(COLOR_CYAN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
    printStatus(*timeStep - 1, *infectedNumber);
}

void runToTime(int *timeStep, int *infectedNumber, int targetTime) {
    if(targetTime <= *timeStep) {
        printf(COLOR_RED "⚠ Target time must be greater than current time (%d)\n" COLOR_RESET, *timeStep);
        return;
    }

    printf("\n" COLOR_MAGENTA COLOR_BOLD "▶ Running to T=%d..." COLOR_RESET "\n\n", targetTime);

    int startTime = *timeStep;
    int duration = targetTime - startTime;

    while(*timeStep <= targetTime) {
        runStep(1.0f);
        infectVehicules(*timeStep, infectedNumber);

        if((*timeStep - startTime) % 2 == 0 || *timeStep == targetTime) {
            printProgressBar(*timeStep - startTime, duration, *infectedNumber);
        }

        (*timeStep)++;
    }

    printf("\n\n" COLOR_GREEN COLOR_BOLD "✓ Reached T=%d" COLOR_RESET "\n", targetTime);
    printf(COLOR_CYAN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
    printStatus(*timeStep - 1, *infectedNumber);
}

int main(){
    srand(time(NULL));
    int timeStep = 0;
    int infectedNumber = 0;
    int choice;
    int targetTime, startNode, endNode;

    printHeader();

    printf("\n" COLOR_CYAN "⚙ Initializing simulation..." COLOR_RESET "\n");

    initVehicules();
    Vehicule *p0 = initPatientZero();
    allVehicules[0] = *p0;

    for(int i=1; i<MAX_VEHICULES; i++) {
        initVehicule(&allVehicules[i]);
    }

    initGraph();

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Vehicles: %d | Danger Radius: %d", MAX_VEHICULES, DANGER_RADIUS);
    printBox("Configuration", buffer);

    printf(COLOR_GREEN "✓ Initialization complete!\n" COLOR_RESET);

    while(1) {
        printMenu();

        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf(COLOR_RED "⚠ Invalid input! Please enter a number.\n" COLOR_RESET);
            continue;
        }

        switch(choice) {
            case 1:
                runCompleteSimulation(&timeStep, &infectedNumber);
                break;

            case 2:
                printf("Enter target time: ");
                scanf("%d", &targetTime);
                runToTime(&timeStep, &infectedNumber, targetTime);
                break;

            case 3:
                if(timeStep == 0) {
                    printf(COLOR_RED "⚠ Please run simulation first!\n" COLOR_RESET);
                } else {
                    analyzeClusters();
                }
                break;

            case 4:
                if(timeStep == 0) {
                    printf(COLOR_RED "⚠ Please run simulation first!\n" COLOR_RESET);
                } else {
                    computeBackboneMST();
                }
                break;

            case 5:
                if(timeStep == 0) {
                    printf(COLOR_RED "⚠ Please run simulation first!\n" COLOR_RESET);
                } else {
                    identifyCriticalVehicles();
                }
                break;

            case 6:
                if(timeStep == 0) {
                    printf(COLOR_RED "⚠ Please run simulation first!\n" COLOR_RESET);
                } else {
                    printf("Enter start vehicle index (0-%d): ", MAX_VEHICULES-1);
                    scanf("%d", &startNode);
                    printf("Enter target vehicle index (0-%d): ", MAX_VEHICULES-1);
                    scanf("%d", &endNode);

                    if(startNode >= 0 && startNode < MAX_VEHICULES &&
                       endNode >= 0 && endNode < MAX_VEHICULES) {
                        findFastestPath(startNode, endNode);
                    } else {
                        printf(COLOR_RED "⚠ Invalid vehicle indices!\n" COLOR_RESET);
                    }
                }
                break;

            case 7:
                printStatus(timeStep, infectedNumber);
                break;

            case 0:
                printf("\n" COLOR_CYAN COLOR_BOLD);
                printf("╔═══════════════════════════════════════════════════════════╗\n");
                printf("║                   SIMULATION COMPLETE                     ║\n");
                printf("╚═══════════════════════════════════════════════════════════╝\n");
                printf(COLOR_RESET);
                printStatus(timeStep, infectedNumber);
                printf("\n" COLOR_GREEN "Thank you for using V2V Simulation!\n" COLOR_RESET);
                free(allVehicules);
                return 0;

            default:
                printf(COLOR_RED "⚠ Invalid choice! Please select 0-7.\n" COLOR_RESET);
        }
    }

    return 0;
}
