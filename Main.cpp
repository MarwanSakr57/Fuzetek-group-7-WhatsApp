#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// ========================
//       USER CLASS
// ========================
class User
{
private:
    string username;
    string password;
    string phoneNumber;
    string status;
    string lastSeen;

public:
    User()
    {
        username = "";
        password = "";
        phoneNumber = "";
        status = "Offline";
    }

    User(string uname, string pwd, string phone)
    {
        username = uname;
        password = pwd;
        phoneNumber = phone;
        status = "Offline";
    }

    string getUsername() const
    {
        return username;
    }

    string getPhoneNumber() const
    {
        return phoneNumber;
    }

    string getStatus() const
    {
        return status;
    }

    string getLastSeen() const
    {
        return lastSeen;
    }

    void setStatus(string newStatus)
    {
        status = newStatus;
    }

    void setPhoneNumber(string phone)
    {
        phoneNumber = phone;
    }

    void updateLastSeen()
    {
        time_t now = time(0);
        lastSeen = ctime(&now);
    }

    bool checkPassword(string pwd) const
    {
        return password == pwd;
    }

    void changePassword(string newPwd)
    {

        // TODO: Implement password change
    }
};

// ========================
//      MESSAGE CLASS
// ========================
class Message
{
private:
    string sender;
    string content;
    string timestamp;
    string status;
    Message *replyTo;

public:
    Message()
    {
        sender = "";
        content = "";
        timestamp = "";
        status = "Sent";
        replyTo = nullptr;
        updateTimestamp();
    }

    Message(string sndr, string cntnt) {

    Message(string sndr, string cntnt)
    {
        sender = sndr;
        content = cntnt;
        status = "Sent";
        replyTo = nullptr;
        updateTimestamp();
    }

    string getContent() const
    {
        return content;
    }

    string getSender() const
    {
        return sender;
    }

    string getTimestamp() const
    {
        return timestamp;
    }

    string getStatus() const
    {
        return status;
    }

    Message *getReplyTo() const
    {
        return replyTo;
    }

    void setStatus(string newStatus)
    {
        status = newStatus;
    }

    void setReplyTo(Message *msg)
    {
        replyTo = msg;
    }

    void updateTimestamp()
    {
        time_t now = time(0);
        timestamp = ctime(&now);
        if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back(); // this removes newline char in the end so displaychat() has correct formatting
    }
    }

    void display() const
    {
        cout << "[" << timestamp << "] ";
        cout << sender << ": " << content;
        if (replyTo != nullptr)
        {
            cout << " (reply to: " << replyTo->getContent() << ")";
        }
        cout << " [" << status << "]";
        cout << endl;
    }

    void addEmoji(string emojiCode)
    {
        content += " " + emojiCode;
    }

    void markDelivered() { status = "Delivered"; }
    void markSeen() { status = "Seen"; }
};

// ========================
//       CHAT CLASS (BASE)
// ========================
class Chat
{
protected:
    vector<string> participants;
    vector<Message> messages;
    string chatName;

public:
    Chat()
    {
        participants = {};
        messages = {};
        chatName = "";
    }

    Chat(vector<string> users, string name)
    {
    {
        participants = users;
        chatName = name;
        messages = {};
    }
    const vector<string> &getParticipants() const
    {
        return participants;
    }
    vector<Message>& getMessages() {
        return messages;
    }
    string getChatName() const {
        return chatName;
    }
    void addMessage(const Message &msg)
    {
        messages.push_back(msg);
    }

    void deleteMessage(int index, const string &username)
    {

        if (messages[index].getSender() == username)
        {
            cout << "Message : " << messages[index].getContent() << " Deleted successfuly !" << endl;
            messages.erase(messages.begin() + index);
            return ;
        }
        cout << "user can only delete their messages" << endl;
        return ;
    }

    virtual void displayChat() const
    {
        for (Message msg : messages)
        {
            msg.display();
        }
    }

    vector<Message> searchMessages(string keyword) const
    {
        // TODO: Implement message search
        return {};
    }

    void exportToFile(const string& filename) const {
       ofstream outputFile(filename+".txt");
    if (outputFile.is_open()) {
        for (const Message& msg : messages) {
            outputFile << "[" << msg.getTimestamp() << "] ";
            outputFile << msg.getSender() << ": " << msg.getContent();
            if (msg.getReplyTo() != nullptr) {
                outputFile << " (reply to: " << msg.getReplyTo()->getContent() << ")";
            }
            outputFile << " [" << msg.getStatus() << "]";
            outputFile << endl;
        }
    } else {
        // Handle error, e.g., print an error message
        std::cerr << "Error opening file!" << std::endl;
    }
    outputFile.close();
    }
};

// ========================
//     PRIVATE CHAT CLASS
// ========================
class PrivateChat : public Chat
{
private:
    string user1;
    string user2;

public:
    PrivateChat(string u1, string u2)
        : Chat({u1, u2}, u1 + " & " + u2), user1(u1), user2(u2) { }

    void displayChat() const override {
        cout << "Private Chat between " << user1 << " and " << user2 << endl;
        cout << "------------------------------------" << endl;
        Chat::displayChat();
        cout << "------------------------------------" << endl;
    }

    void showTypingIndicator(const string &username) const
    {
        cout << username << " is typing..." << endl;
    }
};

// ========================
//      GROUP CHAT CLASS
// ========================
class GroupChat : public Chat
{
private:
    vector<string> admins;
    string description;
    string creator;

public:
    GroupChat(vector<string> users, string name, string creatorName)
    {   
        participants=users;
        chatName=name;
        creator = creatorName;
        // byt2kd en l creator mn dmn l nas l fl list
        bool found = false;
        for (int i = 0; i < participants.size(); i++)
        {
            if (participants[i] == creator)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            participants.push_back(creator);
        }

        // hndy l l creator l admin access
        admins.push_back(creator);

        // default description
        description = "";
    }
    bool isCreator(const string& username)const
    {
        return username==creator;
    }

    void removeAdmin(const string& requester, const string& adminToRemove)
    {
        if (!isCreator(requester))
        {
            cout << "Only creator can remove admins.\n";
            return ;
        }
        if (adminToRemove == creator)
        {
            cout << "Creator cannot be removed from admins.\n";
            return ;
        }
        for (size_t i = 0; i < admins.size(); ++i)
        {
            if (admins[i] == adminToRemove)
            {
                admins.erase(admins.begin() + i);
                cout << adminToRemove << " removed from admins.\n";
                return ;
            }
        }
        cout << "Admin not found.\n";
        return ;
    }
    void addParticipant(const string& requester, const string& userToAdd)
    {
        if (!isAdmin(requester)&&!isCreator(requester))
        {
            cout << "Only admins can add participants.\n";
            return ;
        }
        if (!isParticipant(userToAdd))
        {
            participants.push_back(userToAdd);
            cout << userToAdd << " added to group.\n";
            return ;
        }
        cout << "User already in group.\n";
        return ;
    }
    void removeParticipant(const string &admin, const string &userToRemove)
    {   
        if (userToRemove==creator)
        {
            cout<<"Can't remove Group Creator"<<endl;
            return;
        }
        if (!isAdmin(admin))
        {
            cout << admin << " is not an admin.\n";
            return ;
        }
        for (int i = 0; i < participants.size(); i++)
        {
            if (participants[i] == userToRemove)
            {
                participants.erase(participants.begin() + i);
                cout << userToRemove << " removed from group.\n";
                return ;
            }
        }
        cout << userToRemove << " not found in group.\n";
        return ;
    }

    bool isAdmin(string username) const
    {
        for (int i = 0; i < admins.size(); i++)
        {
            if (admins[i] == username)
            {
                return true;
            }
        }
        return false;
    }

    bool isParticipant(string username) const
    {
        for (int i = 0; i < participants.size(); i++)
        {
            if (participants[i] == username)
            {
                return true;
            }
        }
        return false;
    }

    void setDescription(string desc)
    {
        description = desc;
    }
    void deleteMessage(int index, const string& username)
    {
        if (isAdmin(username))
        {
            if (index >= 0 && index < messages.size())
            {
                cout << "Message: " << messages[index].getContent() << " deleted successfully!\n";
                messages.erase(messages.begin() + index);
                return ;
            }
            cout << "Invalid message index.\n";
            return ;
        }
        cout << "Only admins can delete messages in Group.\n";
        return ;
    }
    // void sendGroupMessage(const string &sender, const string &text)
    // {
    //     if (!isParticipant(sender))
    //     {
    //         cout << "Not in group.\n";
    //         return;
    //     }
    //     Message m(sender, text);
    //     m.updateTimestamp();
    //     m.setStatus("Sent");
    //     addMessage(m);
    // }
    void displayChat() const override
    {
        cout << "\nGroup: " << chatName << "\nAdmins: ";
        for (size_t i = 0; i < admins.size(); ++i)
        {
            cout << admins[i];
            if (i != admins.size() - 1)
                cout << ", ";
        }
        cout << "\nMembers: ";
        for (size_t i = 0; i < participants.size(); ++i)
        {
            cout << participants[i];
            if (i != participants.size() - 1)
                cout << ", ";
        }
        cout << "\nMessages:\n";
        for (auto &m : messages)
            m.display();
    }

    void sendJoinRequest(const string &username)
    {
        cout << username << " requested to join group \"" << chatName << "\".\n";
    }
};

// ========================
//    WHATSAPP APP CLASS
// ========================
class WhatsApp
{
private:
    vector<User> users;
    vector<Chat *> chats;
    int currentUserIndex;

    int findUserIndex(string username) const
    {
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].getUsername() == username)
            {
                return i;
            }
        }
        return -1;
    }

    bool isLoggedIn() const
    {
        return currentUserIndex != -1;
    }

    string getCurrentUsername() const
    {
        return users[currentUserIndex].getUsername();
    }

public:
    WhatsApp() : currentUserIndex(-1) {}

    void signUp()
    {
        string uname;
        string pass;
        string phone;
        cin.ignore();
        while (true)
        {
            cout << "Enter Username :" << endl;
            getline(cin, uname);
            if (uname.empty())
            { // checks if user input is blank
                cout << "Please enter valid username" << endl;
            }
            else
            {
                break;
            }
        }
        while (true)
        {
            cout << "Enter Password :" << endl;
            getline(cin, pass);
            if (pass.empty())
            {
                cout << "Please enter valid password" << endl;
            }
            else
            {
                break;
            }
        }
        while (true)
        {
            cout << "Enter Phone Number :" << endl;
            getline(cin, phone);
            if (phone.empty())
            {
                cout << "Please enter valid phone number" << endl;
                continue;
            }

            bool invalid = false;
            int count = 0;
            for (char c : phone)
            {
                if (c < '0' || c > '9')
                { // check if input is all numbers
                    invalid = true;
                    break;
                }
                count++;
            }

            if (invalid || count != 11)
            {
                cout << "Please enter a valid phone number (11 digits only)." << endl;
                continue;
            }
            break;
        }
        User newuser(uname, pass, phone);
        users.push_back(newuser);
        cout << "new user registered!" << endl;
    }

    void login() {
        string username, password;
        cin.ignore();
        cout << "Enter your username: "<< endl;
        getline(cin, username);
        cout << "Enter your password: "<< endl;
        getline(cin, password);


        for (int i=0; i<users.size(); ++i){
            if (users[i].getUsername()==username && users[i].checkPassword(password)){
            if (users[i].getUsername()==username && users[i].checkPassword(password)){
                currentUserIndex = i;
                users[i].setStatus("Online");
                cout << "Login successful " << username <<"!"<< endl;
                cout << "Login successful " << username <<"!"<< endl;
                return;
            }
        }
        cout << "Incorrect username or password. Please try again." << endl;
        cout << "Incorrect username or password. Please try again." << endl;
    }

    void startPrivateChat()
    {
        string reciepient;
        cout << "Enter reciepient name :" << endl;
        cin.ignore();
        getline(cin, reciepient);
        int reciepientIndex = findUserIndex(reciepient);

        if (reciepientIndex == -1)
        {
            cout << "user not found" << endl;
            return;
        }

        string currentUser = getCurrentUsername();
        PrivateChat *chat = new PrivateChat(currentUser, reciepient);
        chats.push_back(chat);
        
        cout << "Chat started with "<<reciepient<<" !" << endl;
        while(true){
        cout<<"type your message (or 'o' for options)"<<endl;
        string messageText;
        getline(cin, messageText);
        if(messageText =="o") {
            cout<<"\n1. Exit chat\n2. Display chat\n3. Export Chat \n ";
            string messageText;
            getline(cin, messageText);
            if(messageText =="1") break;
            if(messageText =="2") {chat->displayChat();continue;}
            if(messageText =="3") {
                string filename;
                cout << "Enter filename to export chat: ";
                getline(cin, filename);
                chat->exportToFile(filename);
                cout << "Chat exported to " << filename << endl;
                continue;
        }
        }
        Message msg=Message(currentUser,messageText);
        msg.markDelivered();
        chat->addMessage(msg);
        }
    }

    void createGroup()
    {
        string name;
        cout << "Group name: ";
        cin.ignore();
        getline(cin, name);

        vector<string> members;
        members.push_back(getCurrentUsername());

        string uname;
        cout << "Enter other members to add (type 'done' to finish):\n";
        while (true)
        {
            getline(cin, uname);
            if (uname == "done")
                break;
            if (findUserIndex(uname) != -1)
                members.push_back(uname);
            else
                cout << "User not found.\n";
        }

        chats.push_back(new GroupChat(members, name, getCurrentUsername()));
        cout << "Group \"" << name << "\" created.\n";
    }

    void viewChats() const
    {
        string currentUser = getCurrentUsername();
        vector<int> userChatIndices;

        cout << "Your Chats:" << endl;
        for (int i = 0; i < chats.size(); ++i)
        {
            Chat *chat = chats[i];
            bool isParticipant = false;
            for (const string &participant : chat->getParticipants())
            {
                if (participant == currentUser)
                {
                    isParticipant = true;
                    break;
                }
            }
            if (isParticipant)
            {
                cout << userChatIndices.size() + 1 << ". " << chat->getChatName() << endl;
                userChatIndices.push_back(i);
            }
        }

        if (userChatIndices.empty())
        {
            cout << "No chats found." << endl;
            return;
        }

        cout << "Enter chat number to open (or 0 to go back): ";
        int choice;
        cin >> choice;
        if (choice < 1 || choice > userChatIndices.size())
            return;

        int chatIndex = userChatIndices[choice - 1];
        Chat* selectedChat = chats[chatIndex];

        for (Message& msg : selectedChat->getMessages()) {
            if (msg.getSender() != currentUser && msg.getStatus() == "Delivered") {
                msg.markSeen();
            }
        }

        // Chat loop
        cin.ignore();
        while(true){
        cout<<"type your message: (or 'o' for options)"<<endl;
        string messageText;
        getline(cin, messageText);
                if(messageText =="o") {
            cout<<"\n1. Exit chat\n2. Display chat\n3. Export Chat \n ";
            string messageText;
            getline(cin, messageText);
            if(messageText =="1") break;
            if(messageText =="2") {selectedChat->displayChat();continue;}
            if(messageText =="3") {
                string filename;
                cout << "Enter filename to export chat: ";
                getline(cin, filename);
                selectedChat->exportToFile(filename);
                cout << "Chat exported to " << filename << endl;
                continue;
        }
        }
        Message msg=Message(currentUser,messageText);
        msg.markDelivered();
        selectedChat->addMessage(msg);
        
        }
    }

    void logout()
    {
        users[currentUserIndex].setStatus("Offline");
        users[currentUserIndex].updateLastSeen();
        cout << getCurrentUsername() << " Logged out successfully";
        currentUserIndex = -1;
    }

    void run()
    {
        while (true)
        {
            if (!isLoggedIn())
            {
                cout << "\n1. Login\n2. Sign Up\n3. Exit\nChoice: ";
                int choice;
                cin >> choice;
                if (choice == 1)
                    login();
                else if (choice == 2)
                    signUp();
                else if (choice == 3)
                    break;
            }
            else
            {
                cout << "\n1. Start Private Chat\n2. Create Group\n3. View Chats\n4. Logout\nChoice: ";
                int choice;
                cin >> choice;
                if (choice == 1)
                    startPrivateChat();
                else if (choice == 2)
                    createGroup();
                else if (choice == 3)
                    viewChats();
                else if (choice == 4)
                    logout();
            }
        }
    }
};

// ========================
//          MAIN
// ========================
int main()
{
    WhatsApp whatsapp;
    whatsapp.run();
    return 0;
}

