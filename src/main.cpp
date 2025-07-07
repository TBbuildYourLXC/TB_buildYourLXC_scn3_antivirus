#include <iostream>
#include <random>
#include <thread>
#include <iomanip>
#include <cstring>
#include <atomic>
#include <array>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

void displayAsciiArt() {
    cout << R"(
 █████╗ ███╗   ██╗████████╗██╗██╗   ██╗██╗██████╗ ██╗   ██╗███████╗
██╔══██╗████╗  ██║╚══██╔══╝██║██║   ██║██║██╔══██╗██║   ██║██╔════╝
███████║██╔██╗ ██║   ██║   ██║██║   ██║██║██████╔╝██║   ██║███████╗
██╔══██║██║╚██╗██║   ██║   ██║╚██╗ ██╔╝██║██╔══██╗██║   ██║╚════██║
██║  ██║██║ ╚████║   ██║   ██║ ╚████╔╝ ██║██║  ██║╚██████╔╝███████║
╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═══╝  ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
         PRO 2025 - PROTECTION ULTIME SUR LINUX
)" << endl;
}

class AntiVirusPro2025 {
private:
    bool fastMode = false;

    void progressBar() {
        for (int i = 0; i <= 100; i += 5) {
            cout << "\rScanning... " << setw(3) << i << "% complete  " << flush;
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }

    void simulateScan() {
        random_device rd;
        mt19937 gen(rd());

        int totalFiles;
        if (fastMode) {
            totalFiles = 100;
        } else {
            uniform_int_distribution<> distFiles(10000, 250000);
            totalFiles = distFiles(gen);
        }

        cout << "\n\033[1;32m[AntiVirus Pro 2025]\033[0m Système de scan initialisé\n"
             << (fastMode ? "Analyse rapide en cours...\n" : "Analyse complète en cours...\n");

        for (int i = 1; i <= totalFiles; ++i) {
            cout << "\rAnalyse fichier " << i << "/" << totalFiles << " en cours     " << flush;
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }

    void securityReport() {
        cout << "\n\n\033[1;32m[RAPPORT DE SECURITE]\033[0m\n"
             << "Aucune menace détectée\n"
             << "Système protégé et sécurisé\n";
    }

public:
    void launch(bool fast = false) {
        fastMode = fast;
        displayAsciiArt();
        cout << "\033[1;34m--- AntiVirus Pro 2025 ---\033[0m\n"
             << (fast ? "Démarrage du scan rapide..." : "Démarrage du scan complet...") << endl;

        simulateScan();
        progressBar();
        securityReport();
    }

    static void displayHelp() {
        cout << "AntiVirus Pro 2025 - Manuel d'utilisation\n"
             << "-----------------------------------------\n"
             << "Options disponibles :\n"
             << "  --help                : Affiche ce menu d'aide détaillé.\n"
             << "  --version             : Affiche le numéro de version du logiciel.\n"
             << "  --fast                : Effectue un scan rapide (limité à 100 fichiers).\n"
             << "\nExemples :\n"
             << "  ./antivirus\n"
             << "      Lance un scan complet\n"
             << "  ./antivirus --fast\n"
             << "      Lance un scan rapide\n"
             << "  ./antivirus --help\n"
             << "      Affiche ce menu d'aide\n";
    }

    static void displayVersion(){
        const string VERSION = "2.0.0";
        cout << "AntiVirus Pro 2025 - Version " << VERSION
             << " © Alexandre Shyshmarov "<< endl;
    }

};


atomic<bool> should_stop{false};

void signal_handler(int signum) {
    if (signum == SIGINT) {
        cout << "\nCtrl+C reçu. Arrêt...\n";
        should_stop = true;
    }
}



int main(int argc, char* argv[]) {

    AntiVirusPro2025 antivirus;
    if (argc == 1){
        antivirus.launch();
    }else{
        if (strcmp(argv[1],"--fast") == 0) {
            antivirus.launch(true);
        } else if (strcmp(argv[1],"--help") == 0) {
            AntiVirusPro2025::displayHelp();
            return 0;
        } else if (strcmp(argv[1],"--version") == 0) {
            AntiVirusPro2025::displayVersion();
            return 0;
        } else {
            std::cerr << "Argument invalide : " << ". Utilisez --help pour l'aide." << std::endl;
            return 1;
        }

    }

    // Section virus
    cout << "PID processus parent : " << getpid() << endl;

    const int NB_ENFANT = 4 ;
    array<pid_t, NB_ENFANT> pids{};
    signal(SIGINT, signal_handler);

    // Créer 4 processus enfants
    for (size_t i = 0; i < NB_ENFANT; ++i) {
        pids[i] = fork();

        if (pids[i] == 0) {
            cout << "PID processus enfant : " << getpid() << endl;
            // Processus enfant - boucle infinie
            while (!should_stop);
            return 0;
        }
    }

    FILE *file = fopen("io_write", "w");
    if (!file) {
        perror("fopen");
        return 1;
    }

    // Désactiver le buffering
    setbuf(file, NULL);

    // Écrire des données en boucle
    char data[] = "AAAAAAAAAA";
    while (!should_stop) {
        fwrite(data, 1, sizeof(data), file);
        fflush(file);  // Forcer l'écriture
        fsync(fileno(file));  // Forcer l'écriture sur le disque
    }

    fclose(file);


    // Terminer les enfants
    for (auto pid : pids) {
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
    }

    return 0;
}
