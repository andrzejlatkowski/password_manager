//
// Created by Andrzej Łątkowski on 12/01/2023.
//
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
namespace fs=std::filesystem;

struct Haslo{
   static int  ID;
   int         id;
   string      name;
   string      login;
   string      password;
   string      category;
   string      website;
   static int  line_counter;

public:
   Haslo(string name, string login, string password) {
       this->name      = name;
       this->login     = login;
       this->password  = password;
       this->category  = "x";
       this->website   = "x";
       id              = ID++;
       line_counter    +=7;
   }
   Haslo(string name, string login, string password, string category, string website) {
       this->name      = name;
       this->login     = login;
       this->password  = password;
       this->category  = category;
       this->website   = website;
       id              = ID++;
       line_counter    +=7;
   }
   ~Haslo() { cout << "Dtor()" << id << endl; }

/**->
 * Deklaracja
 * Getters and setters
 */
int getId() const;
void setId(int id);
const string &getName() const;
void setName(const string &name);
const string &getLogin() const;
void setLogin(const string &login);
const string &getPassword() const;
void setPassword(const string &password);
const string &getCategory() const;
void setCategory(const string &category);
const string &getWebsite() const;
void setWebsite(const string &website);
static int getLineCounter();
static void setLineCounter(int lineCounter);
};

int Haslo::ID = 0;
int Haslo::line_counter = 0;

/**->
 * Definicja
 * Getters and setters
 */
int Haslo::getId() const {
return id;
}
void Haslo::setId(int id) {
Haslo::id = id;
}
const string &Haslo::getName() const {
return name;
}
void Haslo::setName(const string &name) {
Haslo::name = name;
}
const string &Haslo::getLogin() const {
return login;
}
void Haslo::setLogin(const string &login) {
Haslo::login = login;
}
const string &Haslo::getPassword() const {
return password;
}
void Haslo::setPassword(const string &password) {
Haslo::password = password;
}
const string &Haslo::getCategory() const {
return category;
}
void Haslo::setCategory(const string &category) {
Haslo::category = category;
}
const string &Haslo::getWebsite() const {
return website;
}
void Haslo::setWebsite(const string &website) {
Haslo::website = website;
}
int Haslo::getLineCounter() {
return line_counter;
}
void Haslo::setLineCounter(int lineCounter) {
line_counter = lineCounter;
}

/**->
 * Deklaracja metody liczącej hasła w pliku
 * @param file_name nazwa pliku
 * @return
 */
int count_passwords_in_file(string file_name);

//==================FILEFILEFILE========================================================================================

/**->
 * Metoda szyfrująca tekst - XOR. Operator modulo jest używany, aby zapewnić,
 * że indeks hasła nie przekracza rozmiaru tablicy,
 * co oznacza, że gdy indeks przekroczy rozmiar tablicy,
 * jest on "zawijany" do początku tablicy i kontynuowane jest dalsze przetwarzanie.
 * @param text
 * @param password
 */
void encrypt(string& text, const string password) {
    int password_index = 0;
    for (auto &c : text) {
        c ^= password[password_index++ % password.size()];
    }
}
/**->
 * Medoda odszyfrowująca
 * @param text
 * @param password
 */
void decrypt(string& text, const string password) {
    int password_index = 0;
    for (auto &c : text) {
        c ^= password[password_index++ % password.size()];
    }
}
/**->
 * Szyfrowanie pliku
 * @param file_name
 * @param password
 */
void encrypt_file(const string file_name, const string password){
    string path_text = fs::current_path().parent_path().append(file_name);
    ifstream input(path_text);

    // Otwarcie pliku do zaszyfrowania
    ifstream plaintext(path_text);
    string data((istreambuf_iterator<char>(plaintext)), istreambuf_iterator<char>());

    // Zaszyfrowanie danych
    encrypt(data, password);

    // Zapisanie zaszyfrowanych danych
    ofstream ciphertext(path_text);
    ciphertext << data;

    ciphertext.close();
    plaintext.close();
}
/**->
 * Odszyfrowanie pliku
 * @param file_name
 * @param password
 */
void decrypt_file(const string file_name, const string password) {
    string path_text = fs::current_path().parent_path().append(file_name);
    ifstream input(path_text);

    // Otwarcie pliku do zaszyfrowania
    ifstream plaintext(path_text);
    string data((istreambuf_iterator<char>(plaintext)), istreambuf_iterator<char>());

    // Zapisanie zaszyfrowanych danych
    ofstream ciphertext(path_text);
    ciphertext << data;
    ciphertext.close();
    plaintext.close();

    // Odszyfrowanie danych
    plaintext.open(path_text);
    data.clear();
    data.assign((istreambuf_iterator<char>(plaintext)), istreambuf_iterator<char>());
    decrypt(data, password);

    // Zapisanie odszyfrowanych danych
    ciphertext.open(path_text);
    ciphertext << data;

    ciphertext.close();
    plaintext.close();
}
/**->
 * wprowadzanie nowego hasła do pliku
 * @param haslo
 * @param file_name
 */
void input_password(const Haslo* haslo, const string file_name) {
    fs::path path(fs::current_path());
    ofstream output(path.parent_path().append(file_name), ios::app);

    output << haslo->getId() << endl;
    output << haslo->getName() << endl;
    output << haslo->getLogin() << endl;
    output << haslo->getPassword() << endl;
    output << haslo->getCategory() << endl;
    output << haslo->getWebsite() << endl;
    output << "===========================" << endl;

    output.close();
}
/**->
 * uaktalnia plik tekstowy z hasłami na podstawie wektora
 * @param vector
 * @param file_name
 */
void update_password_list(const vector<vector<string>>& vector, const string file_name){

    string path_text = fs::current_path().parent_path().append(file_name);
    ofstream file;
    file.open(path_text);

    for (auto row : vector) {
        for (auto item : row) {
            file << item << endl;
        }
    }

    file.close();
}
/**->
 * drukuje hasła z pliku
 * @param path
 */
void get_passwords(const string path){

    string path_text = fs::current_path().parent_path().append(path);
    ifstream input(path_text);

    string s;
    while (std::getline(input, s))
        cout << s << endl;

    input.close();
}

//==================VECTORVECTOR========================================================================================

/**->
 * Tworzy wektor na podstawie pliku tekstowego
 * @param file_neme nazwa pliku, z którego czerpie dane
 * @return zwraca wektor na podstawie pliku tekstowego
 */
vector<vector<string>> set_vector_password(const string file_neme){

    string path_text = fs::current_path().parent_path().append(file_neme);
    ifstream input(path_text);
    int passwords_counter = count_passwords_in_file(file_neme);

    vector<vector<string>> lista_hasel(passwords_counter, vector<string>(7));

    for (int i=0; i<passwords_counter; i++){
        for (int j = 0; j<7; j++) {
            input >> lista_hasel[i][j];
        }
    }

    input.close();
    return lista_hasel;
}
/**->
 * drukuje nieposortowane hasła z wektora
 * @param vector
 */
void print_password_list(const vector<vector<string>>& vector) {
    for (int i=0; i < vector.size(); i++){
        for (int j=0; j<7; j++) {
            cout << vector[i][j] << endl;
        }
    }
}
/**->
 * drukuje elementy hasła wyszukane na podstawie konkretnego inputu użytkownika - ID
 * @param vector
 * @param id_to_find
 */
void print_password_by_id(const vector<vector<string>>& vector, const int id_to_find) {

    for (int i=0; i < vector.size(); i++){
        if (vector[i][0] == to_string(id_to_find)){
            for (int j=0; j<7; j++) {
                cout << vector[i][j] << endl;
            }
            return;
        }
    }
    cout << "No password found" << endl;
}
/**->
 * drukuje elementy hasła wyszukane na podstawie konkretnego inputu użytkownika [ np NAME: Anabelle ]
 * @param vector
 * @param input_string
 * @param criterium
 */
void print_password_by_string(const vector<vector<string>>& vector, const string input_string, const int criterium) {

    bool found = false;

    for (int i=0; i < vector.size(); i++){
        if (vector[i][criterium] == input_string){
            for (int j=0; j<7; j++) {
                cout << vector[i][j] << endl;
            }
            found = true;
        }
    }
    if (found) return;
    else cout << "No password found" << endl;
}
/**->
 * sprawdzanie czy hasło istnieje już w wektorze
 * @param vector
 * @param password_to_find
 * @return
 */
bool check_if_password_already_exist(const vector<vector<string>>& vector, const string password_to_find) {
    string password = password_to_find;
    bool found = false;

    for (int i=0; i < vector.size(); i++){
        if (vector[i][3] == password_to_find){
            found = true;
        }
    }
    return found;
}
/**->
 * sortowanie wektora na podstawie jednego kryterium oraz drukowanie
 * @param vector
 * @param criterium
 */
void print_sorted_vector_one(const vector<vector<string>>& vector, const int criterium){
    int choice = criterium;

    ::vector<::vector<string>> vector_to_sort = vector;

    sort(vector_to_sort.begin(), vector_to_sort.end(),
         [&choice](const std::vector<string>& a, const std::vector<string>& b) {
        return a[choice] < b[choice];
    });

    for (int i=0; i < vector_to_sort.size(); i++){
        for (int j=0; j<7; j++) {
            cout << vector_to_sort[i][j] << endl;
        }
    }
}
/**->
 * sortowanie wektora na podstawie dwóch kryteriów oraz drukowanie
 * @param vector
 * @param criterium1
 * @param criterium2
 */
void print_sorted_vector_two(const vector<vector<string>>& vector, const int criterium1, const int criterium2){
    int choice1 = criterium1;
    int choice2 = criterium2;

    ::vector<::vector<string>> vector_to_sort = vector;

    sort(vector_to_sort.begin(), vector_to_sort.end(),
         [&choice1, &choice2](const std::vector<string>& a, const std::vector<string>& b) {
        if(a[choice1] != b[choice1]) {  return a[choice1] < b[choice1]; }
                                        return a[choice2] < b[choice2];});

    for (int i=0; i < vector_to_sort.size(); i++){
        for (int j=0; j<7; j++) {
            cout << vector_to_sort[i][j] << endl;
        }
    }
}
/**->
 * Metoda służąca przede wszystkim do drukowania wszystkich danych znalezionych po ID
 * @param vector wektor do skanowania
 * @param id ID
 * @return zwraca znalezione ID
 */
int print_if_found_password_to_edit(const vector<vector<string>>& vector, const int id) {
    int i;
    for (i=0; i < vector.size(); i++){
        if (vector[i][0] == to_string(id)){
            for (int j=0; j<7; j++) {
                cout << vector[i][j] << endl;
            }
            return i;
        }
    }
    cout << "No password found" << endl;
    return -1;
}
/**->
 * nadpisuje daną w wektorze
 * @param vector wektor, który będzie modyfikowany
 * @param id ID hasła
 * @param criterium kryterium do modyfikacji [ nazwa, login etc. ]
 * @param new_data nowa dana do wstawienia w miejscu starej
 */
void edit_found_password_to_edit(vector<vector<string>>& vector, const int id, const int criterium, const string new_data) {
    vector.at(id).at(criterium) = new_data;
}

//==================ADDITIONAL==========================================================================================

/**->
 * @param file_name nazwa pliku
 * @return liczba haseł w pliku
 */
int count_passwords_in_file(const string file_name){
    string path_text = fs::current_path().parent_path().append(file_name);
    ifstream input(path_text);

    int password_counter = 0;
    std::string unused;
    while ( std::getline(input, unused) )
        ++password_counter;
    password_counter/=7;

    input.close();
    return password_counter;
}
/**->
 * automatyczne generowanie hasła na podstawie podanych parametrów
 * @param length ilość znaków
 * @param special_characters czy są znaki specjalne
 * @param small_big_letters  wielkość liter
 * @return wygenerowane hasło
 */
string generate_password(const int length, const int special_characters, const int small_big_letters) {

    char password[length+1];
    int special = special_characters;   // 1 with, 2 without special characters
    int kind = small_big_letters;       // 1 small, 2 big, 3 small and big letters

    char special_small_chars[]          = "abcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=";
    char special_big_chars[]            = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=";
    char special_small_and_big_chars[]  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=";

    char small_chars[]                  = "abcdefghijklmnopqrstuvwxyz0123456789";
    char big_chars[]                    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char small_and_big_chars[]          = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    if (special == 1){
        switch(kind){
            // with special characters
            case 1:                                         //1 - small letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof special_small_chars - 1);
                    password[i] = special_small_chars[randNum];

                }
                break;
            case 2:                                         //2 - big letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof special_big_chars - 1);
                    password[i] = special_big_chars[randNum];
                }
                break;
            case 3:                                         //3 - small and big letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof special_small_and_big_chars - 1);
                    password[i] = special_small_and_big_chars[randNum];
                }
                break;
            default:
                return "1";
        }
    } else if (special == 2){
        switch (kind) {
            // without special characters
            case 1:                                         //1 - small letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof small_chars - 1);
                    password[i] = small_chars[randNum];
                }
                break;
            case 2:                                         //2 - big letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof big_chars - 1);
                    password[i] = big_chars[randNum];
                }
                break;
            case 3:                                         //3 - small and big letters
                for (int i = 0; i < length; i++) {
                    int randNum = rand() % (sizeof small_and_big_chars - 1);
                    password[i] = small_and_big_chars[randNum];
                }
                break;
            default:
                return "1";
        }
    } else {
        return "1";
    }

    password[length] = '\0';
    string to_return = password;
    return to_return;
}
/**->
 * @param password hasło do sprawdzenia
 * @return siła [ 1 - 5 ]
 */
int check_password_strength(const string password){

    int length = password.length();
    char password_tab[length+1];

    char small_letter_tab[] = "abcdefghijklmnopqrstuvwxyz";
    bool small_letter = false;

    char big_letter_tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    bool big_letter = false;

    char special_character_tab[] = "!@#$%^&*()_+-=";
    bool special_character = false;

    char numer_tab[] = "1234567890";
    bool number = false;

    int strength = 0;

    if (length > 7) strength++;
    for (int i = 0; i<length; i++) {
        password_tab[i] = password.at(i);
    }
    for (int i=0; i< size(password); i++) {
        for (char j : small_letter_tab) {
            if (password_tab[i] == j && !small_letter){
                small_letter = true;
                strength++;
            }
        }
        for (char j : big_letter_tab) {
            if (password_tab[i] == j && !big_letter){
                big_letter = true;
                strength++;
            }
        }
        for (char j : special_character_tab) {
            if (password_tab[i] == j && !special_character){
                special_character = true;
                strength++;
            }
        }
        for (char j : numer_tab) {
            if (password_tab[i] == j && !number){
                number = true;
                strength++;
            }
        }
    }
    return strength;
}
/**->
 * @param strength_input [ 1 - 5 ] sila hasla
 * @return string ze słownym poziomem siły hasła
 */
string check_password_strength_to_string(const int strength_input){
    int strength = strength_input;
    if (strength == 1) return "weak";
    if (strength == 2) return "medium";
    if (strength == 3) return "strong";
    if (strength == 4) return "super strong";
    if (strength == 5) return "extreme strong";
}

//======================================================================================================================

int main() {

    string file_name = "a_hasla.txt";

    string path_text = fs::current_path().parent_path().append(file_name);
    ifstream input(path_text);

    string password = "password";
    string password_enter;

    cout << "Enter password: ";
    cin >> password_enter;

    // hasło do programu
    if (password_enter != password) {
        bool correct = false;
        while (!correct) {
            cout << "Exit program, enter [ 1 ]" << endl;
            cout << "Wrong password, enter password: ";
            cin >> password_enter;
            if (password_enter == password) correct = true;
            if (password_enter == "1") return 0;
        }
    }

    int menu_select = -1;
    int second_menu_select;
    string string_input;
    string string_password;
    string string_name;
    string string_login;
    string string_category;
    string string_website;

    // odkodowuje plik
    decrypt_file(file_name, password);

    // tworzy wektor na podstawie pliku
    vector<vector<string>> lista_hasel = set_vector_password(file_name);

    // CURRENT ID
    Haslo::ID = count_passwords_in_file(file_name);

    // szyfrowanie pliku
    encrypt_file(file_name, password);

    input.close();

    while (menu_select != 0) {

        cout << endl;
        cout << "Menu:" << endl;
        cout << "1. Search password" << endl;
        cout << "2. Sort passwords" << endl;
        cout << "3. Add password" << endl;
        cout << "4. Edit password" << endl;
//        cout << "5. Delete password" << endl;     // TODO
//        cout << "6. Add category" << endl;        // TODO
//        cout << "7. Delete category" << endl;     // TODO
        cout << "0. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> menu_select;

        while (menu_select == 1) {
            cout << "Search by:" << endl;
            cout << "1. Id" << endl;
            cout << "2. Name" << endl;
            cout << "3. Login" << endl;
            cout << "4. Password" << endl;
            cout << "5. Category" << endl;
            cout << "6. Website" << endl;
            cout << "0. Back" << endl;
            cout << "Enter your choice: ";
            cin >> second_menu_select;
            while(second_menu_select == 1){
                cout << "Enter Id number: ";
                int select;
                cin >> select;
                print_password_by_id(lista_hasel, select);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            while(second_menu_select == 2){
                cout << "Enter name: ";
                cin >> string_input;
                print_password_by_string(lista_hasel, string_input, 1);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            while(second_menu_select == 3){
                cout << "Enter login: ";
                cin >> string_input;
                print_password_by_string(lista_hasel, string_input, 2);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            while(second_menu_select == 4){
                cout << "Enter password: ";
                cin >> string_input;
                print_password_by_string(lista_hasel, string_input, 3);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            while(second_menu_select == 5){
                cout << "Enter category: ";
                cin >> string_input;
                print_password_by_string(lista_hasel, string_input, 4);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            while(second_menu_select == 6){
                cout << "Enter website: ";
                cin >> string_input;
                print_password_by_string(lista_hasel, string_input, 5);
                cout << "0. Go back: ";
                int go_back;
                cin >> go_back;
                while (go_back != 0) {
                    cout << "0. Go back: ";
                    cin >> go_back;
                }
                break;
            }
            if (second_menu_select == 0) break;
        }       // WORKS ON VECTOR
        while (menu_select == 2) {
            cout << "Sort by:" << endl;
            cout << "1. One Parameter"  << endl;
            cout << "2. Two Parameters" << endl;
            cout << "3. Print passwords from file" << endl;
            cout << "0. Back" << endl;
            cout << "Enter your choice: ";
            cin >> second_menu_select;

            while (second_menu_select == 1) {
                cout << "Sort by:" << endl;
                cout << "1. Id" << endl;
                cout << "2. Name" << endl;
                cout << "3. Login" << endl;
                cout << "4. Password" << endl;
                cout << "5. Category" << endl;
                cout << "6. Website" << endl;
                cout << "0. Back" << endl;
                cout << "Enter your choice: ";
                int select;
                cin >> select;

                if (select != 1 && select != 2 && select != 3 && select != 4 && select != 5 && select != 6 &&select != 0 ){
                    cout << "Wrong input" << endl;
                    continue;
                }
                if (select == 0) break;

                print_sorted_vector_one(lista_hasel, (select - 1));
            }

            while (second_menu_select == 2) {
                bool first_choice_check = false;
                bool second_choice_check = false;
                int select1;
                int select2;
                if (!first_choice_check) {
                    cout << "Sort by first criterium:" << endl;
                    cout << "1. Id" << endl;
                    cout << "2. Name" << endl;
                    cout << "3. Login" << endl;
                    cout << "4. Password" << endl;
                    cout << "5. Category" << endl;
                    cout << "6. Website" << endl;
                    cout << "0. Back" << endl;
                    cout << "Enter your choice: ";

                    cin >> select1;

                    if (select1 != 1 && select1 != 2 && select1 != 3 && select1 != 4 && select1 != 5 && select1 != 6 && select1 != 0 ){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    if (select1 == 0) break;
                    first_choice_check = true;
                }
                while (!second_choice_check) {
                    cout << "Sort by second criterium:" << endl;
                    cout << "1. Id" << endl;
                    cout << "2. Name" << endl;
                    cout << "3. Login" << endl;
                    cout << "4. Password" << endl;
                    cout << "5. Category" << endl;
                    cout << "6. Website" << endl;
                    cout << "0. Back" << endl;
                    cout << "Enter your choice: ";

                    cin >> select2;

                    if (select2 != 1 && select2 != 2 && select2 != 3 && select2 != 4 && select2 != 5 && select2 != 6 && select2 != 0 ){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    if (select2 == 0) break;
                    second_choice_check = true;
                    print_sorted_vector_two(lista_hasel, (select1 - 1), (select2 - 1));
                }
                break;
            }

            if (second_menu_select == 3) {
                decrypt_file(file_name, password);
                get_passwords(file_name);
                encrypt_file(file_name, password);
            }

            if (second_menu_select != 1 && second_menu_select != 2 && second_menu_select != 3 && second_menu_select != 0 ){
                cout << "Wrong input" << endl;
                continue;
            }

            if (second_menu_select == 0) break;
        }       // WORKS ON VECTOR
        while (menu_select == 3) {
            cout << "Add new:" << endl;
            cout << "1. My own password" << endl;
            cout << "2. Generated password" << endl;
            cout << "0. Back" << endl;
            cout << "Enter your choice: ";
            cin >> second_menu_select;

            bool password_strength_approval = false;
            bool login_check = false;
            bool name_check = false;
            bool category_check = false;
            bool website_check = false;
            bool all_saved_check = false;

            if (second_menu_select != 1 && second_menu_select != 2 && second_menu_select != 0 ){
                cout << "Wrong input" << endl;
                continue;
            }
            if (second_menu_select == 0) break;

            while(!all_saved_check){
                if (!password_strength_approval) {
                    if (second_menu_select == 1) {
                        cout << "Enter new password: ";
                        cin >> string_password;

                        if (check_if_password_already_exist(lista_hasel, string_password)){
                            cout << "Password already exists" << endl;
                            continue;
                        }
                        string password_strength = check_password_strength_to_string(
                                check_password_strength(string_password));
                        if (size(string_password) < 5) {
                            cout << "Password too short" << endl;
                            cout << "0. Go back: ";
                            int go_back;
                            cin >> go_back;
                            while (go_back != 0) {
                                cout << "0. Go back: ";
                                cin >> go_back;
                            }
                            continue;
                        }
                        if (check_password_strength(string_password) < 3) {
                            cout << "Password strength is '" << password_strength << "', please enter stronger password"
                                 << endl;
                            cout << "0. Go back: ";
                            int go_back;
                            cin >> go_back;
                            while (go_back != 0) {
                                cout << "0. Go back: ";
                                cin >> go_back;
                            }
                            continue;
                        }
                        cout << "Password strength is '" << password_strength << "'" << endl;
                        password_strength_approval = true;
                    }
                    if (second_menu_select == 2) {
                        int special_characters;
                        int small_big_letters;
                        int length;
                        bool characters_check = false;
                        bool small_big_letters_check = false;
                        bool length_check = false;

                        while (!characters_check) {
                            cout << "Generate password: " << endl;
                            cout << "1. With special characters" << endl;
                            cout << "2. Without special characters" << endl;
                            cout << "Enter your choice: ";

                            cin >> special_characters;
                            if (special_characters != 1 && special_characters != 2) {
                                cout << "Wrong input" << endl;
                                cout << "0. Go back: ";
                                int go_back;
                                cin >> go_back;
                                while (go_back != 0) {
                                    cout << "0. Go back: ";
                                    cin >> go_back;
                                }
                                continue;
                            }
                            characters_check = true;
                        }
                        while (!small_big_letters_check){
                            cout << "Generate password: " << endl;
                            cout << "1. With small letters" << endl;
                            cout << "2. With big letters" << endl;
                            cout << "3. With small and big letters" << endl;
                            cout << "Enter your choice: ";

                            cin >> small_big_letters;
                            if (small_big_letters != 1 && small_big_letters != 2 && small_big_letters != 3) {
                                cout << "Wrong input" << endl;
                                cout << "0. Go back: ";
                                int go_back;
                                cin >> go_back;
                                while (go_back != 0) {
                                    cout << "0. Go back: ";
                                    cin >> go_back;
                                }
                                continue;
                            }
                            small_big_letters_check = true;
                        }
                        while (!length_check) {
                            cout << "Generate length: [ not less than 8 letters ]" << endl;

                            cin >> length;
                            if (length < 8) {
                                cout << "Wrong input" << endl;
                                cout << "0. Go back: ";
                                int go_back;
                                cin >> go_back;
                                while (go_back != 0) {
                                    cout << "0. Go back: ";
                                    cin >> go_back;
                                }
                                continue;
                            }
                            length_check = true;
                        }

                        string_password = generate_password(length, special_characters, small_big_letters);
                        cout << "Your generated password: " << string_password << endl;
                        if (check_if_password_already_exist(lista_hasel, string_password)){
                            cout << "Password already exists" << endl;
                            continue;
                        }
                        string password_strength = check_password_strength_to_string(
                                check_password_strength(string_password));
                        cout << "Password strength is '" << password_strength << "'" << endl;
                        password_strength_approval = true;
                    }
                }
                cout << "Enter:" << endl;
                cout << "1. Name" << endl;
                cout << "2. Login" << endl;
                cout << "3. Category" << endl;
                cout << "4. Website" << endl;
                cout << "0. Save and go back" << endl;
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;
                if (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 0 ){
                    cout << "Wrong input" << endl;
                    continue;
                }
                while(choice == 1 ){
                    cout << "Enter name: ";
                    cin >> string_name;
                    cout << "Name is: " << string_name << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "Name is saved!" << endl;
                        name_check = true;
                        break;
                    }
                    if (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 2 ){
                    cout << "Enter login: ";
                    cin >> string_login;
                    cout << "Login is: " << string_login << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "Login is saved!" << endl;
                        login_check = true;
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 3 ){
                    cout << "Enter category: ";
                    cin >> string_category;
                    cout << "Category is: " << string_category << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "Category is saved!" << endl;
                        category_check = true;
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 4 ){
                    cout << "Enter website: ";
                    cin >> string_website;
                    cout << "Website is: " << string_website << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "Website is saved!" << endl;
                        website_check = true;
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 0) {

                    if (password_strength_approval && login_check && name_check && category_check && website_check) {
                        decrypt_file(file_name, password);
                        input_password(new Haslo(string_name, string_login, string_password,
                                                 string_category, string_website), file_name);
                        lista_hasel = set_vector_password(file_name);
                        cout << "All data is saved, password Id is: " << Haslo::ID -1<< endl;
                        all_saved_check = true;
                        encrypt_file(file_name, password);
                        break;
                    }
                    if (password_strength_approval && login_check && name_check && category_check) {
                        decrypt_file(file_name, password);
                        input_password(new Haslo(string_name, string_login, string_password,
                                                 string_category, "x"), file_name);
                        lista_hasel = set_vector_password(file_name);
                        cout << "All data is saved, password Id is: " << Haslo::ID -1<< endl;
                        all_saved_check = true;
                        encrypt_file(file_name, password);
                        break;
                    }
                    if (password_strength_approval && login_check && name_check && website_check) {
                        decrypt_file(file_name, password);
                        input_password(new Haslo(string_name, string_login, string_password,
                                                 "x", string_website), file_name);
                        lista_hasel = set_vector_password(file_name);
                        cout << "All data is saved, password Id is: " << Haslo::ID -1 << endl;
                        all_saved_check = true;
                        encrypt_file(file_name, password);
                        break;
                    }
                    if (password_strength_approval && login_check && name_check) {
                        decrypt_file(file_name, password);
                        input_password(new Haslo(string_name, string_login, string_password),
                                       file_name);
                        lista_hasel = set_vector_password(file_name);
                        cout << "All data is saved, password Id is: " << Haslo::ID -1 << endl;
                        all_saved_check = true;
                        encrypt_file(file_name, password);
                        break;
                    }
                    if (!name_check) {
                        cout << "Please enter name" << endl;
                        break;
                    }
                    if (!login_check) {
                        cout << "Please enter login." << endl;
                        break;
                    }
                }
            }
            break;
        }       // WORKS ON VECTOR AND FILE
        if (menu_select == 4) {
            cout << "Enter password Id: ";
            int select;
            cin >> select;

            int found_id = print_if_found_password_to_edit(lista_hasel, select);
            bool password_strength_approval = false;

            while(found_id != -1) {
                cout << "Edit:" << endl;
                cout << "1. Name" << endl;
                cout << "2. Login" << endl;
                cout << "3. Password" << endl;
                cout << "4. Category" << endl;
                cout << "5. Website" << endl;
                cout << "0. Save and go back" << endl;
                cout << "Enter your choice: ";
                int choice;
                cin >> choice;
                if (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 0 ){
                    cout << "Wrong input" << endl;
                    continue;
                }
                string string_data;
                while(choice == 1 ){
                    cout << "Enter new name: ";
                    cin >> string_data;
                    cout << "Name is: " << string_data << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "New name is saved!" << endl;
                        edit_found_password_to_edit(lista_hasel, found_id, choice, string_data);
                        break;
                    }
                    if (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 2 ){
                    cout << "Enter new login: ";
                    cin >> string_data;
                    cout << "Login is: " << string_data << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "New login is saved!" << endl;
                        edit_found_password_to_edit(lista_hasel, found_id, choice, string_data);
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 3 ){
                    cout << "Enter new password:" << endl;
                    cout << "1. Enter own password" << endl;
                    cout << "2. Enter generated password" << endl;
                    cout << "0. Back" << endl;
                    cout << "Enter your choice: ";
                    cin >> second_menu_select;
                    if (second_menu_select != 1 && second_menu_select != 2 && second_menu_select != 0 ){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    if (second_menu_select == 0) break;

                    while (!password_strength_approval) {
                        if (second_menu_select == 1) {
                            cout << "Enter new password: ";
                            cin >> string_password;

                            if (check_if_password_already_exist(lista_hasel, string_password)){
                                cout << "Password already exists" << endl;
                                continue;
                            }
                            string password_strength = check_password_strength_to_string(
                                    check_password_strength(string_password));
                            if (size(string_password) < 5) {
                                cout << "Password too short" << endl;
                                cout << "0. Go back: ";
                                int go_back;
                                cin >> go_back;
                                while (go_back != 0) {
                                    cout << "0. Go back: ";
                                    cin >> go_back;
                                }
                                continue;
                            }
                            if (check_password_strength(string_password) < 3) {
                                cout << "Password strength is '" << password_strength << "', please enter stronger password"
                                     << endl;
                                cout << "0. Go back: ";
                                int go_back;
                                cin >> go_back;
                                while (go_back != 0) {
                                    cout << "0. Go back: ";
                                    cin >> go_back;
                                }
                                continue;
                            }
                            cout << "Password strength is '" << password_strength << "'" << endl;
                            password_strength_approval = true;
                        }
                        if (second_menu_select == 2) {
                            int special_characters;
                            int small_big_letters;
                            int length;
                            bool characters_check = false;
                            bool small_big_letters_check = false;
                            bool length_check = false;

                            while (!characters_check) {
                                cout << "Generate password: " << endl;
                                cout << "1. With special characters" << endl;
                                cout << "2. Without special characters" << endl;
                                cout << "Enter your choice: ";

                                cin >> special_characters;
                                if (special_characters != 1 && special_characters != 2) {
                                    cout << "Wrong input" << endl;
                                    cout << "0. Go back: ";
                                    int go_back;
                                    cin >> go_back;
                                    while (go_back != 0) {
                                        cout << "0. Go back: ";
                                        cin >> go_back;
                                    }
                                    continue;
                                }
                                characters_check = true;
                            }
                            while (!small_big_letters_check){
                                cout << "Generate password: " << endl;
                                cout << "1. With small letters" << endl;
                                cout << "2. With big letters" << endl;
                                cout << "3. With small and big letters" << endl;
                                cout << "Enter your choice: ";

                                cin >> small_big_letters;
                                if (small_big_letters != 1 && small_big_letters != 2 && small_big_letters != 3) {
                                    cout << "Wrong input" << endl;
                                    cout << "0. Go back: ";
                                    int go_back;
                                    cin >> go_back;
                                    while (go_back != 0) {
                                        cout << "0. Go back: ";
                                        cin >> go_back;
                                    }
                                    continue;
                                }
                                small_big_letters_check = true;
                            }
                            while (!length_check) {
                                cout << "Generate length: [ not less than 8 letters ]" << endl;

                                cin >> length;
                                if (length < 8) {
                                    cout << "Wrong input" << endl;
                                    cout << "0. Go back: ";
                                    int go_back;
                                    cin >> go_back;
                                    while (go_back != 0) {
                                        cout << "0. Go back: ";
                                        cin >> go_back;
                                    }
                                    continue;
                                }
                                length_check = true;
                            }

                            string_password = generate_password(length, special_characters, small_big_letters);
                            cout << "Your generated password: " << string_password << endl;
                            if (check_if_password_already_exist(lista_hasel, string_password)){
                                cout << "Password already exists" << endl;
                                continue;
                            }
                            string password_strength = check_password_strength_to_string(
                                    check_password_strength(string_password));
                            cout << "Password strength is '" << password_strength << "'" << endl;
                            password_strength_approval = true;
                        }
                        edit_found_password_to_edit(lista_hasel, found_id, choice, string_password);
                    }
                    cout << "New password is saved!" << endl;
                    break;
                }
                while(choice == 4 ){
                    cout << "Enter new category: ";
                    cin >> string_data;
                    cout << "Category is: " << string_data << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "New category is saved!" << endl;
                        edit_found_password_to_edit(lista_hasel, found_id, choice, string_data);
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                while(choice == 5 ){
                    cout << "Enter new website: ";
                    cin >> string_data;
                    cout << "Website is: " << string_data << endl;
                    cout << "Press:" << endl;
                    cout << "1. Save" << endl;
                    cout << "2. Edit" << endl;
                    cout << "Enter your choice: ";
                    int save_or_edit;
                    cin >> save_or_edit;
                    if (save_or_edit != 1 && save_or_edit != 2){
                        cout << "Wrong input" << endl;
                        continue;
                    }
                    while (save_or_edit == 1) {
                        cout << "New website is saved!" << endl;
                        edit_found_password_to_edit(lista_hasel, found_id, choice, string_data);
                        break;
                    }
                    while (save_or_edit == 2) continue;
                    break;
                }
                if(choice == 0) {

                    decrypt_file(file_name, password);
                    update_password_list(lista_hasel, file_name);

                    cout << "All data is saved!" << endl;
                    encrypt_file(file_name, password);
                    break;
                }
            }
        }          // WORKS ON VECTOR AND FILE

        //TODO while (menu_select == 5) {}
        //TODO while (menu_select == 6) {}
        //TODO while (menu_select == 7) {}
    }

//TODO==================================================================================================================

//    LOCAL TIME
//    time_t now = time(0);
//    tm* local = localtime(&now);
//    cout << local->tm_hour << endl;
//    cout << local->tm_min << endl;
//    cout << local->tm_sec << endl;

    return 0;
}