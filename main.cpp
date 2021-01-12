#include <iostream>

using namespace std;

int main() {
    cout << "Dobro došli!" << endl;
    cout << "Odaberite jednu od ponuđenih opcija unosom rednog broja!" << endl;
    string options[9] = {
        "Unos formule (Opcija omogućuje unos trenutne logičke formule)\n",
        "Unos grafa (Opcija omogućuje unos trenutnog grafa)\n",
        "Je li formula ispunjiva (Opcija omogućuje ispitivanje da li je formula ispunjiva)\n",
        "Postoji li k-nezavisnih čvorova (Opcija ispituje da li u grafu postoji skup sa barem k nezavisnih čvorova)\n",
        "Postoji li k-klika (Opcija ispituje da li u grafu postoji k-klika)\n",
        "Verifikacione formule (Opcija omogućuje verifikaciju nekog pridruživanja vrijednosti logičkim varijablama)\n",
        "Verifikacija skupa nezavisnih čvorova (Opcija omogućuje verifikaciju skupa nezavisnih čvorova)\n",
        "Verifikacija k-klike (Opcija omogućuje verifikaciju k-klike)\n",
        "Izlaz\n"
    };

    for(int i = 0; i < 9; ++i)
        cout << i + 1 << ". " << options[i];

    int input;
    do {
        cout << "Unesite opciju koju želite\n";
        cin >> input;
        switch(input) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                cout << "Odabrali ste prekid programa!\n\n";
                return 0;
            default:
                cout << "Unesena opcija nije validna, unesite ponovo!\n\n";
        }
    } while(input != 9);
    return 0;
}
