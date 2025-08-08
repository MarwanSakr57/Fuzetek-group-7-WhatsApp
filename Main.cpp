#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <limits>
#include <algorithm>
#include <sstream>
using namespace std;

// ========================
//       USER CLASS
// ========================
class User {
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

public:
    User() :  username(""), password(""), phoneNumber(""), status("Offline"), lastSeen("") {}

    User(string uname, string pwd, string phone)
        : username(uname), password(pwd), phoneNumber(phone), status("Offline"), lastSeen("") {}

    string getUsername() const { return username; }
    string getPhoneNumber() const { return phoneNumber; }
    string getStatus() const { return status; }
    string getLastSeen() const { return lastSeen; }

    void setStatus(string newStatus) { status = newStatus; }
    void setPhoneNumber(string phone) { phoneNumber = phone; }

    void updateLastSeen() {
        time_t now = time(0);
        char* dt = ctime(&now);
        lastSeen = string(dt);
        if (!lastSeen.empty() && lastSeen.back() == '\n') lastSeen.pop_back();
    }

    bool checkPassword(string pwd) const {
        return pwd == password;
    }

    void changePassword(string newPwd) {
        password = newPwd;
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message {
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message* replyTo;

public:
    Message() : sender(""), content(""), timestamp(""), status("sent"), replyTo(nullptr) {
        updateTimestamp();
    }

    Message(string sndr, string cntnt) : sender(sndr), content(cntnt), status("sent"), replyTo(nullptr) {
        updateTimestamp();
    }

    string getContent() const { return content; }
    string getSender() const { return sender; }
    string getTimestamp() const { return timestamp; }
    string getStatus() const { return status; }
    Message* getReplyTo() const { return replyTo; }

    void setStatus(string newStatus) { status = newStatus; }
    void setReplyTo(Message* msg) { replyTo = msg; }

    // helper to set timestamp when loading from file
    void setTimestamp(const string& t) { timestamp = t; }

    void updateTimestamp() {
        time_t now = time(0);
        char* dt = ctime(&now);
        timestamp = string(dt);
        if (!timestamp.empty() && timestamp.back() == '\n') timestamp.pop_back();
    }

    void display() const {
        cout << "[" << timestamp << "] " << sender << ": " << content;
        if (replyTo) cout << " (reply to " << replyTo->getSender() << ")";
        cout << endl;
    }

    void addEmoji(string emojiCode) {
        content += " " + emojiCode;
    }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat {
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat() : participants(), messages(), chatName("") {}

    Chat(vector<string> users, string name) : participants(users), messages(), chatName(name) {}

    void addMessage(const Message& msg) {
        messages.push_back(msg);
    }

    bool deleteMessage(int index, const string& username) {
        if (index < 0 || index >= (int)messages.size()) return false;
        if (messages[index].getSender() != username) return false; // only sender can delete
        messages.erase(messages.begin() + index);
        return true;
    }

    virtual void displayChat() const {
        cout << "\n--- " << chatName << " ---\n";
        for (size_t i = 0; i < messages.size(); ++i) {
            cout << i << " ";
            messages[i].display();
        }
        cout << "--------------------\n";
    }

    vector<Message> searchMessages(string keyword) const {
        vector<Message> res;
        for (const auto& m : messages) {
            if (m.getContent().find(keyword) != string::npos) res.push_back(m);
        }
        return res;
    }

    void exportToFile(const string& filename) const {
        ofstream ofs(filename);
        if (!ofs.is_open()) {
            cout << "Cannot open file to export: " << filename << endl;
            return;
        }
        for (const auto& m : messages) {
            // format: timestamp|sender|content
            ofs << m.getTimestamp() << "|" << m.getSender() << "|" << m.getContent() << "\n";
        }
        ofs.close();
        cout << "Exported chat to " << filename << endl;
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;

    // helper to create stable filename (alphabetical, spaces -> _)
    string makeFilename() const {
        string a = user1, b = user2;
        if (a > b) swap(a, b);
        for (auto &c : a) if (c == ' ') c = '_';
        for (auto &c : b) if (c == ' ') c = '_';
        return "chat_" + a + "_" + b + ".txt";
    }

public:
    PrivateChat(string u1, string u2) : user1(u1), user2(u2) {
        participants.clear();
        participants.push_back(u1);
        participants.push_back(u2);
        chatName = u1 + " & " + u2 + " Chat";
    }

    // load messages from file (if exists)
    void loadFromFile() {
        string fn = makeFilename();
        ifstream ifs(fn);
        if (!ifs.is_open()) return;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            size_t p1 = line.find('|');
            if (p1 == string::npos) continue;
            size_t p2 = line.find('|', p1 + 1);
            if (p2 == string::npos) continue;
            string ts = line.substr(0, p1);
            string snd = line.substr(p1 + 1, p2 - p1 - 1);
            string cnt = line.substr(p2 + 1);
            Message m(snd, cnt);
            m.setTimestamp(ts); // keep original timestamp
            messages.push_back(m);
        }
        ifs.close();
    }

    // override addMessage so we append to the file automatically
    void addMessage(const Message& msg) {
        Chat::addMessage(msg);
        string fn = makeFilename();
        ofstream ofs(fn, ios::app);
        if (ofs.is_open()) {
            ofs << msg.getTimestamp() << "|" << msg.getSender() << "|" << msg.getContent() << "\n";
            ofs.close();
        }
    }

    void displayChat() const override {
        cout << "\n--- " << chatName << " ---\n";
        for (const auto& m : messages) m.display();
        cout << "--------------------\n";
    }

    void showTypingIndicator(const string& username) const {
        cout << username << " is typing..." << endl;
    }

    bool involves(const string& a, const string& b) const {
        return ( (user1 == a && user2 == b) || (user1 == b && user2 == a) );
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat {
private:
    vector<string> admins;
    string description;

public:
    // constructor: users = initial participants, name = chat name, creator = initial admin
    GroupChat(vector<string> users, string name, string creator) {
        participants = users;
        chatName = name;
        description = "";
        admins.clear();
        // ensure creator is participant
        if (find(participants.begin(), participants.end(), creator) == participants.end()) {
            participants.push_back(creator);
        }
        admins.push_back(creator);
    }

    void addAdmin(string newAdmin) {
        if (!isParticipant(newAdmin)) {
            cout << "Cannot make \"" << newAdmin << "\" admin: not a participant.\n";
            return;
        }
        if (!isAdmin(newAdmin)) {
            admins.push_back(newAdmin);
            cout << newAdmin << " is now an admin.\n";
        } else {
            cout << newAdmin << " is already an admin.\n";
        }
    }

    bool removeParticipant(const string& admin, const string& userToRemove) {
        // only an admin can remove
        if (!isAdmin(admin)) {
            cout << "Only admins can remove participants.\n";
            return false;
        }
        auto it = find(participants.begin(), participants.end(), userToRemove);
        if (it == participants.end()) {
            cout << userToRemove << " is not in the group.\n";
            return false;
        }
        participants.erase(it);
        // also remove from admins if existed
        auto it2 = find(admins.begin(), admins.end(), userToRemove);
        if (it2 != admins.end()) admins.erase(it2);
        cout << userToRemove << " removed from the group.\n";
        return true;
    }

    bool isAdmin(string username) const {
        return find(admins.begin(), admins.end(), username) != admins.end();
    }

    bool isParticipant(string username) const {
        return find(participants.begin(), participants.end(), username) != participants.end();
    }

    void setDescription(string desc) {
        description = desc;
        cout << "Group description updated.\n";
    }

    void displayChat() const override {
        cout << "\n--- " << chatName << " (Group) ---\n";
        cout << "Description: " << description << "\n";
        cout << "Admins: ";
        for (const auto& a : admins) cout << a << " ";
        cout << "\nParticipants: ";
        for (const auto& p : participants) cout << p << " ";
        cout << "\nMessages:\n";
        for (const auto& m : messages) m.display();
        cout << "--------------------\n";
    }

    void sendJoinRequest(const string& username) {
        // simple implementation: just print request (could be extended to queue requests)
        cout << username << " requested to join the group \"" << chatName << "\".\n";
    }

    // optional: add participant directly (used by admin)
    bool addParticipantByAdmin(const string& admin, const string& newUser) {
        if (!isAdmin(admin)) {
            cout << "Only admins can add participants.\n";
            return false;
        }
        if (isParticipant(newUser)) {
            cout << newUser << " is already a participant.\n";
            return false;
        }
        participants.push_back(newUser);
        cout << newUser << " added to the group.\n";
        return true;
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================

      class WhatsApp {
private:
    vector<User> users;
    vector<Chat*> chats;
    int currentUserIndex;

    int findUserIndex(string username) const {
        for (int i = 0; i < (int)users.size(); ++i) {
            if (users[i].getUsername() == username) return i;
        }
        return -1;
    }

    bool isLoggedIn() const {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const {
        if (currentUserIndex >= 0 && currentUserIndex < (int)users.size())
            return users[currentUserIndex].getUsername();
        return "";
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    void signUp() {
        string uname;
        string pass;
        string phone;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        while(true){
            cout<<"Enter Username :"<<endl;
            getline(cin,uname);
            if(uname.empty()){                  //checks if user input is blank
                cout<<"Please enter valid username"<<endl;
            } else{
                break;
            }
        }
        while(true){
            cout<<"Enter Password :"<<endl;
            getline(cin,pass);
            if(pass.empty()){
                cout<<"Please enter valid password"<<endl;
            } else{
                break;
            }
        }
        while(true){
            cout<<"Enter Phone Number :"<<endl;
            getline(cin,phone);
            if(phone.empty()){
                cout<<"Please enter valid phone number"<<endl;
                continue;
            }

            bool invalid = false;
            int count=0;
            for (char c : phone) {
                if (c < '0' || c > '9') {   //check if input is all numbers
                    invalid = true;
                    break;
                }
                count++;
            }

            if (invalid||count!=11) {
                cout << "Please enter a valid phone number (11 digits only)." << endl;
                continue;
            }
            break;
        }
        User newuser(uname,pass,phone);
        users.push_back(newuser);
        cout<<"new user registered!"<<endl;
    }

    void login() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string uname, pwd;
        cout << "Enter Username: " << endl;
        getline(cin, uname);
        cout << "Enter Password: " << endl;
        getline(cin, pwd);

        int idx = findUserIndex(uname);
        if (idx == -1) {
            cout << "User not found.\n";
            return;
        }
        if (!users[idx].checkPassword(pwd)) {
            cout << "Wrong password.\n";
            return;
        }
        currentUserIndex = idx;
        users[idx].setStatus("Online");
        users[idx].updateLastSeen();
        cout << "Logged in as " << uname << " (status set to Online)" << endl;
    }

    void startPrivateChat() {
        if (!isLoggedIn()) {
            cout << "You must login first.\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string otherUser;
        cout << "Enter username to chat with: ";
        getline(cin, otherUser);

        if (otherUser == getCurrentUsername()) {
            cout << "You can't chat with yourself.\n";
            return;
        }

        int idxOther = findUserIndex(otherUser);
        if (idxOther == -1) {
            cout << "User not found.\n";
            return;
        }

        // try to find existing private chat
        PrivateChat* pc = nullptr;
        for (Chat* c : chats) {
            PrivateChat* p = dynamic_cast<PrivateChat*>(c);
            if (p && p->involves(getCurrentUsername(), otherUser)) {
                pc = p;
                break;
            }
        }

        if (!pc) {
            pc = new PrivateChat(getCurrentUsername(), otherUser);
            pc->loadFromFile(); // load history if exists
            chats.push_back(pc);
        }

        // chat loop
        while (true) {
            pc->displayChat();
            cout << "Type message (or '/exit' to go back): ";
            string msg;
            getline(cin, msg);
            if (msg == "/exit") break;
            if (msg.empty()) continue;

            Message newMsg(getCurrentUsername(), msg);
            pc->addMessage(newMsg);
            cout << "Message sent.\n";
        }
    }

    void createGroup() {
        cout << "Group creation not implemented in detail yet.\n";
    }

    void viewChats() const {
        if (chats.empty()) {
            cout << "No chats yet.\n";
            return;
        }
        cout << "Your chats:\n";
        int idx = 0;
        for (const Chat* c : chats) {
            const PrivateChat* p = dynamic_cast<const PrivateChat*>(c);
            if (p) {
                cout << idx++ << ". " << p->getChatName() << endl;
            } else {
                cout << idx++ << ". " << "(group or unknown chat)" << endl;
            }
        }
    }

    void logout() {
        if (!isLoggedIn()) {
            cout << "No user is logged in.\n";
            return;
        }
        users[currentUserIndex].setStatus("Offline");
        users[currentUserIndex].updateLastSeen();
        cout << "User " << getCurrentUsername() << " logged out.\n";
        currentUserIndex = -1;
    }

    void run() {
        while (true) {
            if (!isLoggedIn()) {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) login();
                else if (choice == 2) signUp();
                else if (choice == 3) break;
            }
            else {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1) startPrivateChat();
                else if (choice == 2) createGroup();
                else if (choice == 3) viewChats();
                else if (choice == 4) logout();
            }
        }
    }
}; 
// ========================
//          MAIN
// ========================
int main() {
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}