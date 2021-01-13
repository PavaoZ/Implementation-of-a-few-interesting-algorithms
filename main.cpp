#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

class Formula3CNF {
private:
    vector<vector<int>> logic_formula_clauses;
    vector<vector<int>> logic_formula_table_of_combinations;
    int n = 0, maxClausesNumber = 8, maxLogicVariables = 24, numberOfLogicVariables = 0;

    bool checkIfTooManyLogicVariables(vector<vector<int>> checkInsideVector, int maxVar) {
        for(int i = 0; i < checkInsideVector.size(); ++i)
            this->numberOfLogicVariables += checkInsideVector[i].size();
        return this->numberOfLogicVariables > maxVar;
    }
public:
    // unos3CNF function
    void input3CNF() {
        // Our formula
        string clauses;
        int index = 0;
        while(getline(cin, clauses) && (index < (this->maxClausesNumber - 1))) {
            // Example input is 1,2,3
            // If no next character is added then our input ends
            if(clauses.empty()) break;
            // For every input we add another vector
            this->logic_formula_clauses.emplace_back();

            // Stream so we can forward our input
            istringstream stream_to_matrix_helper(clauses);
            // We use this to input any numeric input into our matrix
            for (int k; stream_to_matrix_helper >> k;) {
                this->logic_formula_clauses[index].push_back(k);
                if (abs(k) > n)
                    n = abs(k);
                // So we can input something like
                // 1,2,3
                // -2,3,4
                // 3,4,-3
                // etc...
                // And these values would represent a formula like:
                // X1 V X2 V X3
                // -X2 V X3 V X4
                // X3 V X4 V -X3
                // etc...
                if (stream_to_matrix_helper.peek() == ',')
                    stream_to_matrix_helper.ignore();
            }
            // Here we check if we have too many logic variable
            if(checkIfTooManyLogicVariables(this->logic_formula_clauses, this->maxLogicVariables)) {
                cout << "Too many logic variables! " << "Up to " << this->maxLogicVariables << "!" << endl;
                vector<vector<int>> forEmpty;
                this->logic_formula_clauses = forEmpty;
                this->numberOfLogicVariables = 0;
                break;
            }
            // We change our row in matrix into which we input so that
            // 1,2,3 goes into one row
            // -2,3,4 goes into second row
            // 3,4,-3 goes into third row
            // etc...
            index++;

            // 1,2,3 is for example a clause with 3 literals
        }

        this->combinationsTable();
    }

    void combinationsTable() {
        // 1 << n shows us how many combinations we have for length of bit string n
        vector<vector<int>> output(n, vector<int>(1 << n));

        // 1U is an unsigned value with the single bit 0 set,
        // and all the other bits cleared.
        // The << operator means "shift to the left".
        // 1U << 0 means create a value with bit 0 set;
        // 1U << 1 means create a value with bit 1 set; etc...
        unsigned num_to_fill = 1U << (n - 1);
        for(unsigned col = 0; col < n; ++col, num_to_fill >>= 1U)
            for(unsigned row = num_to_fill; row < (1U << n); row += (num_to_fill * 2))
                fill_n(&output[col][row], num_to_fill, 1);

        logic_formula_table_of_combinations = output;
    }

    // rjesenje3CNF function
    bool solvable3CNF() {
        // We say for a clause that it is solvable if I(C) = 1 (all subclauses when put together through logical conjuction give 1/true)
        // this would be for example (1 << n) -> 16
        bool solvable = false;
        for(unsigned x = 0; x < (1 << n); ++x) {
            vector<int> help_vector;
            // this would be for example n -> 4
            for(unsigned y = 0; y < n; ++y)
                help_vector.push_back(this->logic_formula_table_of_combinations[y][x]);
            // [y][x] because we need to go through every out of the (1 << n) combinations

            if(checkClauseCombination(help_vector)) {
                // when does this return true?
                // for example:
                // input [X1,X2,X3], [X2,X3,X1], [X1,X1,X1]
                // it would only return true if we know that (X1 V X2 V X3) conj (X2 V X3 V X1) conj (X1 V X1 V X1) is equal to true so that every single subclause is true
                // if we find a combination that is ok we can stop the execution
                solvable = true;
                break;
            }
        }

        return solvable;
    }

    // Help function
    bool checkClauseCombination(vector<int> possibleCombinations) {
        bool acceptableCombination = true;

        for(int i = 0; i < this->logic_formula_clauses.size(); ++i) {
            bool clauseCheck = false;
            for(int j = 0; j < this->logic_formula_clauses[i].size(); ++j) {
                // because we input values for the formula starting from 1, we won't really have a case where the input is [0,1,2] for example but always or similar to [1,2,3]
                // possibleCombinations contains for example [X1 -> 0, X2 -> 0, X3 -> 1], but the indexes in possibleCombinations are from 0 so that why we take the absolute vaue - 1
                const int index = abs(this->logic_formula_clauses[i][j]) - 1;
                if(this->logic_formula_clauses[i][j] > 0 && possibleCombinations[index]) {
                    clauseCheck = true;
                    break;
                } else if(this->logic_formula_clauses[i][j] < 0 && !possibleCombinations[index]) {
                    clauseCheck = true;
                    break;
                }
            }

            // Comment from StackOverFlow
            // In short, you should go with whichever version is the easiest to read and maintain.
            // In slightly older times, I know breaking out of a loop was considered to be a no-no (on par with a goto statement).
            // Loops were supposed to break on the loop condition and nowhere else. Thus, the while-loop would have been the way to go.
            // (This is probably a holdover from assembly, where loops are basically a block of code with a go-to-the-beginning-if-true jump statement at the end.
            // Multiple conditional-jump statements in the block make it exceedingly hard to debug; thus they were to be avoided and combined into one at the end.)
            // Maybe better practice to set j to this->logic_formula_clauses.size() but doesn't really matter
            if(!clauseCheck) {
                acceptableCombination = false;
                break;
            }
        }

        return acceptableCombination;
    }

    // verifikacija3CNF
    bool verification3CNF(std::vector<int> possibleCombinations) {
        return checkClauseCombination(possibleCombinations);
    }
};

// KlikaINDSETGraf
class CliqueINDSETGraph {
private:
    // For a subgraph we say it is a clique if all the nodes of the subgraph are connected (for Clique)
    // We call it a k - clique if k >= 2 (k -> number of nodes)
    std::vector<std::vector<int>> graphMatrix, graphMatrix2, matrixNeighbors, matrixNeighbors2;

    // provjera independent seta tj. k vrijednosti independent seta
    // koristeci algoritam bojenje cvorova tj. pamcenje setova povezanosti (obojanih)
    // i eliminisuci susjede
    // ovaj algoritam se moze svesti i na k-klika problem, samo ako se obrnu matrice tj.
    // values 0 i 1 pri unosu i naprave nove, kao sto je uradjeno ovdje
public:
    // unosGrafa
    void inputGraph(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2) {
        this->graphMatrix = matrix1;
        this->graphMatrix2 = matrix2;

        // We find here the neighbors, we save the indexes of the neighbors
        for(int i = 0; i < this->graphMatrix.size(); ++i) {
            std::vector<int> neighbor;
            for (int j = 0; j < this->graphMatrix[i].size(); ++j)
                if(this->graphMatrix[i][j] == 1)
                    neighbor.push_back(j);
            this->matrixNeighbors.push_back(neighbor);
        }

        // We find here the neighbors, we save the indexes of the neighbors
        for (int i = 0; i < this->graphMatrix2.size(); ++i) {
            std::vector<int> neighbor;
            for (int j = 0; j < this->graphMatrix2[i].size(); ++j)
                if(this->graphMatrix2[i][j] == 1)
                    neighbor.push_back(j);
            this->matrixNeighbors2.push_back(neighbor);
        }
    }

    // Continue work from here on to bottom
    // rjesenjekINDSET
    bool solutionkINDSET(int k) {
        // for some number k searches if the inputted graph has at least k independent nodes
        bool found = false;
        int howManyINDNodes = this->graphMatrix.size() - k;
        // minimal independent set/k - clique number
        int min = this->graphMatrix.size() + 1;

        std::vector<int> helpVec;

        // default value initialization
        for(int i = 0; i < this->graphMatrix.size(); ++i)
            helpVec.push_back(1);

        for(int i = 0; i < helpVec.size(); ++i) {
            std::vector<int> newHelpVec = helpVec;
            newHelpVec[i] = 0;

            newHelpVec = eliminisiSveSusjedneCvoroveUSetu(this->matrixNeighbors, newHelpVec);
            int s = dajBrojCvorovaPovezanosti(newHelpVec); // poziv za dobijanje broja povezanih cvorova
            if (s < min) min = s;
            if (s <= k) {
                found = true;
                break;
            }

            for (int j = 0; j < k; j++)
                newHelpVec = provjeraVanjskihCvorova(this->matrixNeighbors, newHelpVec, j);
            s = dajBrojCvorovaPovezanosti(newHelpVec);
            if (s < min)
                min = s;
            if (s <= k) {
                found = true;
                break;
            }
        }

        return found;
    }

    std::vector<int> removeNeighborNode(std::vector<std::vector<int>> &susjedi, std::vector<int> setCvorova) {
        std::vector<int> tempSet = std::move(setCvorova);
        int r = 0;
        while (r != -1) {
            r = maxPovezanostCvorova(susjedi, tempSet);
            if (r != -1) tempSet[r] = 0;
        }
        return tempSet;
    }
};

int main() {
    Formula3CNF initial;
    CliqueINDSETGraph initialQINDSETG;
    int helpVar, k;
    vector<int> helpVec, resetHelpVec;
    vector<vector<int>> graphMatrix, graphMatrixSecond, resetMatrixHelp;

    cout << "Dobro došli!" << endl;
    cout << "Odaberite jednu od ponuđenih opcija unosom rednog broja!" << endl;
    string options[11] = {
        "Unos formule (Opcija omogućuje unos trenutne logičke formule)\n",
        "Unos grafa (Opcija omogućuje unos trenutnog grafa)\n",
        "Je li formula ispunjiva (Opcija omogućuje ispitivanje da li je formula ispunjiva)\n",
        "Postoji li k-nezavisnih čvorova (Opcija ispituje da li u grafu postoji skup sa barem k nezavisnih čvorova)\n",
        "Postoji li k-klika (Opcija ispituje da li u grafu postoji k-klika)\n",
        "Verifikacione formule (Opcija omogućuje verifikaciju nekog pridruživanja vrijednosti logičkim varijablama)\n",
        "Verifikacija skupa nezavisnih čvorova (Opcija omogućuje verifikaciju skupa nezavisnih čvorova)\n",
        "Verifikacija k-klike (Opcija omogućuje verifikaciju k-klike)\n",
        "Redukcija 3-SAT-TO-INDSET\n",
        "Redukcija 3-SAT-TO-CLIQUE\n",
        "Izlaz\n"
    };

    for(int i = 0; i < 11; ++i)
        cout << i + 1 << ". " << options[i];

    int input, rowColumnSize;

    do {
        cout << "Unesite opciju koju želite\n";
        cin >> input;
        switch(input) {
            case 1:
                cout << "Odabrali ste unos formule!\n\n";
                // cin.clear() clears the error flag on cin (so that future I/O operations will work correctly)
                cin.clear();
                // ignores inputs in stream
                cin.ignore();
                initial.input3CNF();
                break;
            case 2:
                cout << "Odabrali ste unos grafa!\n\n";
                cin.clear();
                cin.ignore();
                do {
                    cout << "Unesite broj redova i kolona: " << std::endl;
                    cin >> rowColumnSize;
                    if(rowColumnSize > 5)
                        cout << "Morate manju dimenziju unijeti (do 5)!" << endl;
                } while(rowColumnSize > 5);
                cout << "Unesite elemente matrice: " << std::endl;

                graphMatrix.resize(rowColumnSize);
                graphMatrixSecond.resize(rowColumnSize);

                for(int i = 0; i < rowColumnSize; ++i) {
                    graphMatrix[i].resize(rowColumnSize);
                    graphMatrixSecond[i].resize(rowColumnSize);
                    int j = 0;
                    while(j < rowColumnSize) {
                        int temp;
                        cin >> temp;
                        graphMatrix[i][j] = temp;
                        // We have here two matrices, one for example graphMatrix = [[1,1,0], [0,0,0]] and graphMatrixSecond = [[0,0,1], [1,1,1]]
                        if(!temp)
                            graphMatrixSecond[i][j] = 1;
                        else
                            graphMatrixSecond[i][j] = 0;
                        j++;
                    }
                }

                initialQINDSETG.inputGraph(graphMatrix, graphMatrixSecond);
                graphMatrix = resetMatrixHelp;
                graphMatrixSecond = resetMatrixHelp;
                break;
            case 3:
                cout << "Odabrali ste provjeru ispunjivosti formule!\n\n";
                cin.clear();
                cin.ignore();
                if(initial.solvable3CNF())
                    cout << "Ispunjiva formula!" << endl;
                else
                    cout << "Nije ispunjiva formula!" << endl;
                break;
            case 4:
                cout << "Odabrali ste provjeru ima li k nezavisnih čvorova!\n\n";
                cin.clear();
                cin.ignore();
                cout << "Unesite vrijednost k: " << endl;
                cin >> k;
                if(initialQINDSETG.solutionkINDSET(k))
                    cout << "Postoji!" << endl;
                else
                    cout << "Ne postoji!" << endl;
                break;
            case 5:
                break;
            case 6:
                cout << "Odabrali ste verifikaciju nekog pridruživanja!\n\n";
                while(helpVar != -1) {
                    cin >> helpVar;
                    if(helpVar == -1) break;
                    helpVec.push_back(helpVar);
                }
                if(helpVec.size() > 0) {
                    if(initial.verification3CNF(helpVec))
                        cout << "Ispunjivo za ovo pridruživanje!" << endl;
                    else
                        cout << "Nije ispunjivo za ovo pridruživanje!" << endl;
                } else
                    cout << "Unesite više vrijednosti!" << endl;
                helpVec = resetHelpVec;
                helpVar = 0;
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                cout << "Odabrali ste prekid programa!\n\n";
                return 0;
            default:
                cout << "Unesena opcija nije validna, unesite ponovo!\n\n";
        }
    } while(input != 9);
    return 0;
}
