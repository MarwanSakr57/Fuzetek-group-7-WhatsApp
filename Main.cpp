#include <iostream>
#include <vector>
#include <string>
#include <ctime>
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
    User() {
       username="";
       password="";
       phoneNumber="";
       status="Offline";
    }
    
    User(string uname, string pwd, string phone) {
       username=uname;
       password=pwd;
       phoneNumber=phone;
       status="Offline";
    }
    
    string getUsername() const {
        return username;
    }
    
    string getPhoneNumber() const {
        return phoneNumber;
    }
    
    string getStatus() const {
        return status;
    }
    
    string getLastSeen() const {
        // TODO: Implement getter
        return "";
    }
    
    void setStatus(string newStatus) {
        status =newStatus;
    }
    
    void setPhoneNumber(string phone) {
        phoneNumber=phone;
    }
    
    void updateLastSeen() {
        // TODO: Implement last seen update
    }
    
    bool checkPassword(string pwd) const {
        // TODO: Implement password check
        return false;
    }
    
    void changePassword(string newPwd) {
        // TODO: Implement password change
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
    Message() {
        sender = "";
        content = "";
        timestamp = "";
        status = "Sent";
        replyTo = nullptr;
        updateTimestamp();
    }
    
    Message(string sndr, string cntnt) {
        sender = sndr;
        content = cntnt;
        status = "Sent";
        replyTo = nullptr;
        updateTimestamp();
    }
    
    string getContent() const {
        return content;
    }
    
    string getSender() const {
        return sender;
    }
    
    string getTimestamp() const {
        return timestamp;
    }
    
    string getStatus() const {
        return status;
    }
    
    Message* getReplyTo() const {
        return replyTo;
    }
    
    void setStatus(string newStatus) {
        status = newStatus;
    }
    
    void setReplyTo(Message* msg) {
        replyTo = msg;
    }
    
    void updateTimestamp() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
        timestamp = buf;
    }
    
    void display() const {
        cout << "[" << timestamp << "] ";
        cout << sender << ": " << content;
        if (replyTo != nullptr) {
            cout << " (reply to: " << replyTo->getContent() << ")";
        }
        cout << " [" << status << "]";
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
    Chat() {
        // TODO: Implement default constructor
    }
    
    Chat(vector<string> users, string name) {
        // TODO: Implement parameterized constructor
    }
    
    void addMessage(const Message& msg) {
        // TODO: Implement message addition
    }
    
    bool deleteMessage(int index, const string& username) {
        // TODO: Implement message deletion
        return false;
    }
    
    virtual void displayChat() const {
        // TODO: Implement chat display
    }
    
    vector<Message> searchMessages(string keyword) const {
        // TODO: Implement message search
        return {};
    }
    
    void exportToFile(const string& filename) const {
        // TODO: Implement export to file
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat {
private:
    string user1;
    string user2;
    
public:
    PrivateChat(string u1, string u2) : user1(u1), user2(u2) {
        participants.push_back(u1);
        participants.push_back(u2);
        chatName = u1 + " & " + u2;
    }
    
    void displayChat() const override {
        cout << "Private Chat between " << user1 << " and " << user2 << endl;
        cout << "------------------------------------" << endl;
        for (const auto& msg : messages) {
            msg.display();
        }
        cout << "------------------------------------" << endl;
    }
    
    void showTypingIndicator(const string& username) const {
        cout << username << " is typing..." << endl;
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
    GroupChat(vector<string> users, string name, string creator) {
        // TODO: Implement constructor
    }
    
    void addAdmin(string newAdmin) {
        // TODO: Implement add admin
    }
    
    bool removeParticipant(const string& admin, const string& userToRemove) {
        // TODO: Implement remove participant
        return false;
    }
    
    bool isAdmin(string username) const {
        // TODO: Implement admin check
        return false;
    }
    
    bool isParticipant(string username) const {
        // TODO: Implement participant check
        return false;
    }
    
    void setDescription(string desc) {
        // TODO: Implement set description
    }
    
    void displayChat() const override {
        // TODO: Implement group chat display
    }
    
    void sendJoinRequest(const string& username) {
        // TODO: Implement join request
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
        // TODO: Implement user search
        return -1;
    }
    
    bool isLoggedIn() const {
        // TODO: Implement login check
        return false;
    }
    
    string getCurrentUsername() const {
        // TODO: Implement get current user
        return "";
    }
    
public:
    WhatsApp() : currentUserIndex(-1) {}
    
    void signUp() {
        string uname;
        string pass;
        string phone;
        cin.ignore();
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
        // TODO: Implement user login
    }
    
    void startPrivateChat() {
        // TODO: Implement private chat creation
    }
    
    void createGroup() {
        // TODO: Implement group creation
    }
    
    void viewChats() const {
        // TODO: Implement chat viewing
    }
    
    void logout() {
        // TODO: Implement logout
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