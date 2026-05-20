#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

class Person
{
protected:
    int id;
    string name;
    long long contact;

public:
    virtual bool input() = 0;
    virtual void display() const = 0;
};

class Payment
{
private:
    float amountPaid;
    bool paymentStatus;

public:
    Payment()
    {
        amountPaid = 0;
        paymentStatus = false;
    }

    void makePayment(float amount)
    {
        amountPaid = amount;

        if (amountPaid > 2000)
        {
            paymentStatus = true;
        }
    }

    float getAmountPaid() const
    {
        return amountPaid;
    }

    bool isPaid() const
    {
        return paymentStatus;
    }

    string paymentMessage() const
    {
        if (paymentStatus)
        {
            return "PAID";
        }

        return "NOT PAID";
    }
};

class Member : public Person
{
private:
    string membershipType;
    string timing;
    float fee;

    Payment payment;

public:

    Member()
    {
        id = 0;
        fee = 0;
    }

    bool input() override
    {
        cout << "==== ADD MEMBER ====\n";

        cout << "Enter Member ID: ";
        cin >> id;

        cin.ignore();

        cout << "Enter Member Name: ";
        getline(cin, name);

        cout << "Enter Contact Number: ";
        cin >> contact;

        int choice;

        cout << "\nMembership Packages\n";
        cout << "\n1. Gold Package\n";
        cout << "\n2. Silver Package\n";
        cout << "\nEnter Choice: \n";
        cin >> choice;

        if (choice == 1)
        {
            membershipType = "Gold";
            fee = 2500;
        }
        else
        {
            membershipType = "Silver";
            fee = 2000;
        }

        cout << "\nAvailable Timings\n";
        cout << "\n1. 6AM - 7AM\n";
        cout << "\n2. 7AM - 8AM\n";
        cout << "\n3. 5PM - 6PM\n";
        cout << "\nEnter Choice: \n";

        cin >> choice;

        switch (choice)
        {
        case 1:
            timing = "6AM - 7AM";
            break;

        case 2:
            timing = "7AM - 8AM";
            break;

        case 3:
            timing = "5PM - 6PM";
            break;

        default:
            timing = "Not Selected";
        }


//payment sess
        float amount;

        cout << "\nMembership Fee: KES \n" << fee;
        cout << "\nEnter Payment Amount: KES \n";
        cin >> amount;

        // payment validation
        if (amount < fee)
        {
            cout << "\n====================\n";
            cout << "\nINSUFFICIENT PAYMENT!\n";
            cout << "\nYou must pay the full membership fee. \n";
            cout << "\nRegistration Failed. \n";
            cout << "\n=======================\n";

            return false;
        }

        //success payment
        payment.makePayment(amount);

        cout << "\nPayment Successful!\n";
        cout << "\nMember Registered Successfully!\n";

        return true;
    }

    //function diplay
    void display() const override
    {
        cout << "\n===================\n";
        cout << "\nMember ID        :" << id;
        cout << "\nName             :" << name;
        cout << "\nContact          :" << contact;
        cout << "\nMembership Type  :" << membershipType;
        cout << "\nGym Timing       :" << timing;
        cout << "\nMembership Fee   :KES" << fee;
        cout << "\nAmount Paid      :KES" << payment.getAmountPaid();
        cout << "\nPayment Status   :" << payment.paymentMessage();
        cout << "\n====================\n";
    }

    int getID() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    // save file
    void saveToFile(ofstream &file)
    {
        file << id << endl;
        file << name << endl;
        file << contact << endl;
        file << membershipType << endl;
        file << timing << endl;
        file << fee << endl;
        file << payment.getAmountPaid() << endl;
        file << payment.isPaid() << endl;
    }
    // save file
    void loadFromFile(ifstream &file)
    {
        float amount;
        bool status;

        file >> id;
        file.ignore();

        getline(file, name);

        file >> contact;
        file.ignore();

        getline(file, membershipType);
        getline(file, timing);

        file >> fee;
        file >> amount;
        file >> status;

        file.ignore();

        payment.makePayment(amount);
    }
};

class Admin
{
private:
    string username;
    string password;

public:
    Admin()
    {
        username = "admin";
        password = "gym123";
    }

    bool login()
    {
        string u, p;

        cout << "\n===== ADMIN LOGIN =====\n";

        cout << "Username: ";
        cin >> u;

        cout << "Password: ";
        cin >> p;

        if (u == username && p == password)
        {
            cout << "Login Successful!\n";
            return true;
        }

        cout << "Invalid Username or Password!\n";
        return false;
    }
};

class GymManagement
{
private:
    vector<Member> members;
    stack<Member> deletedMembers;

public:
    void addMember()
    {
        Member member;

        // If payment is valid, save a member
        if (member.input())
        {
            members.push_back(member);

            saveData();
        }
    }

    // dispaly members
    void displayMembers() const
    {
        if (members.empty())
        {
            cout << "No Members Found!\n";
            return;
        }

        for (const auto &member : members)
        {
            member.display();
        }
    }

    // search a member
    void searchMember() const
    {
        int id;

        cout << "Enter Member ID to Search: ";
        cin >> id;

        for (const auto &member : members)
        {
            if (member.getID() == id)
            {
                cout << "Member Found!\n";

                member.display();

                return;
            }
        }

        cout << "Member Not Found!\n";
    }

    // delete member
    void deleteMember()
    {
        int id;

        cout << "Enter Member ID to Delete: \n";
        cin >> id;

        for (auto it = members.begin(); it != members.end(); ++it)
        {
            if (it->getID() == id)
            {
                deletedMembers.push(*it);

                members.erase(it);

                saveData();

                cout << "Member Deleted Successfully!\n";

                return;
            }
        }

        cout << "Member Not Found!\n";
    }
    // undo delete a member
    void undoDelete()
    {
        if (deletedMembers.empty())
        {
            cout << "No Deleted Members To Restore!\n";
            return;
        }

        members.push_back(deletedMembers.top());

        deletedMembers.pop();

        saveData();

        cout << "Deleted Member Restored Successfully!\n";
    }

    void saveData()
    {
        ofstream file("gym_members.txt");

        for (auto &member : members)
        {
            member.saveToFile(file);
        }

        file.close();
    }

    void loadData()
    {
        ifstream file("gym_members.txt");

        if (!file)
        {
            return;
        }

        while (!file.eof())
        {
            Member member;

            member.loadFromFile(file);

            if (file)
            {
                members.push_back(member);
            }
        }

        file.close();
    }

    void menu()
    {
        int choice;

        do
        {
            cout << "============ GYM MANAGEMENT SYSTEM ===============\n";
            cout << "1. Add Member\n";
            cout << "2. Display All Members\n";
            cout << "3. Search Member\n";
            cout << "4. Delete Member\n";
            cout << "5. Undo Delete\n";
            cout << "6. Exit\n";
            cout << "Enter Choice:\n ";
            cout << "===============================================\n" ;

            cin >> choice ;

            switch (choice)
            {
            case 1:
                addMember();
                break;

            case 2:
                displayMembers();
                break;

            case 3:
                searchMember();
                break;

            case 4:
                deleteMember();
                break;

            case 5:
                undoDelete();
                break;

            case 6:
                cout << "Thank You For Using The System!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
            }

        } while (choice != 6);
    }
};
// main function
int main()
{
    Admin admin;

    // login system
    if (!admin.login())
    {
        return 0;
    }

    GymManagement gym;

    // load saved members
    gym.loadData();

    // the start menu
    gym.menu();

    return 0;
}
