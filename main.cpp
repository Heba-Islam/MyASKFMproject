#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#define pb push_back
using namespace std;
struct user
{
    string Name,Username,Email,Password;
    int ID;
};
struct QA
{
    string qa;
    int ToID, FromID;
};
struct full{
    string q,r;
};
class DataBase
{
protected:
    vector<user>Users_Data;
    vector<QA>Questions;
    vector<QA>Answers;
    vector<full>VAnsweredQuestions;
    map<string,string>History;
public:
    DataBase()//:Users_In("Users.txt"),Questions_In("questions.txt"),Answers_In("answers.txt")
    {
        ifstream Users_In("Users.txt",ios::in);
        ifstream Questions_In("questions.txt",ios::in);
        ifstream Answers_In("answers.txt",ios::in);
        ifstream AnsweredQuestions("AnsweredQuestions.txt",ios::in);
        string u;
        int cntusers=1;
        user MyUser;
        QA MyQuestion;
        while(getline(Users_In,u))
        {
            if(cntusers%5==1){
                MyUser.Name = u;
            }
            else if(cntusers%5==2){
                MyUser.Username = u;
            }
            else if(cntusers%5==3){
                MyUser.Email = u;
            }
            else if(cntusers%5==4){
                MyUser.Password = u;
            }
            else if(cntusers%5==0){
                string id = u;
                int MyNumber=0;
                for(int i=0; i<id.size(); i++){
                    if(id[i]!=' ')
                    {
                       int MyTemp = id[i]-'0';
                       MyNumber = MyNumber*10 + MyTemp;
                    }
                }
                MyUser.ID=MyNumber;
                Users_Data.pb(MyUser);
            }
            cntusers++;
        }
        string qu;
        int cntquestions=1;
        while(getline(Questions_In,qu))
        {
            if(cntquestions%3==1 && !qu.empty()){
                MyQuestion.qa = qu;
                cntquestions++;
            }
            else if(cntquestions%3==2 && !qu.empty())
            {
                int MyNumber = 0;
                string to=qu;
                for(int i=0; i<to.size(); i++){
                    int MyTemp = to[i]-'0';
                    MyNumber = MyNumber*10 + MyTemp;
                }
                MyQuestion.ToID = MyNumber;
                cntquestions++;
            }
            else if(cntquestions%3==0 && !qu.empty()){
                int MyNumber = 0;
                string from=qu;
                for(int i=0; i<from.size(); i++){
                    int MyTemp = from[i]-'0';
                    MyNumber = MyNumber*10 + MyTemp;
                }
                MyQuestion.FromID = MyNumber;
                Questions.pb(MyQuestion);
                cntquestions++;
            }
        }
        string res;
        int cntresponds=1;
        QA MyRespond;
        while(getline(Answers_In,res))
        {
            if(cntresponds%3==1 && !res.empty()){
                MyRespond.qa=res;
                cntresponds++;
            }
            else if(cntresponds%3==2 && !res.empty())
            {
                int MyNumber = 0;
                string to=res;
                for(int i=0; i<to.size(); i++){
                    int MyTemp = to[i]-'0';
                    MyNumber = MyNumber*10 + MyTemp;
                }
                MyRespond.ToID = MyNumber;
                cntresponds++;
            }
            else if(cntresponds%3==0 && !res.empty()){
                int MyNumber = 0;
                string from=res;
                for(int i=0; i<from.size(); i++){
                    int MyTemp = from[i]-'0';
                    MyNumber = MyNumber*10 + MyTemp;
                }
                cntresponds++;
                MyRespond.FromID = MyNumber;
                Answers.pb(MyRespond);
            }
        }
        string thread;
        int cntlines=1;
        full th;
        while(getline(AnsweredQuestions,thread)){
            if(cntlines%2==1 && !thread.empty()){
                th.q=thread;
                cntlines++;
            }
            else if(cntlines%2==0 && !thread.empty()){
                th.r=thread;
                cntlines++;
                VAnsweredQuestions.pb(th);
                History[th.q]=th.r;
            }
        }

    }
    void UploadUsers()
    {
        ofstream Users_Out("Users.txt",ios::trunc);
        for(int i=0; i<Users_Data.size(); i++){
            Users_Out<<Users_Data[i].Name<<"\n"<<Users_Data[i].Username<<"\n";
            Users_Out<<Users_Data[i].Email<<"\n"<<Users_Data[i].Password<<"\n"<<Users_Data[i].ID<<"\n";
        }
    }
    void UploadQuestions()
    {
       ofstream Questions_Out ("questions.txt",ios::trunc);
        for(int i=0; i<Questions.size(); i++){
            Questions_Out<<Questions[i].qa<<"\n"<<Questions[i].ToID<<"\n"<<Questions[i].FromID<<"\n";
        }
    }
    void UploadAnswers()
    {
        ofstream Answers_Out ("answers.txt",ios::trunc);
        for(int i=0; i<Answers.size(); i++){
            Answers_Out<<Answers[i].qa<<"\n"<<Answers[i].ToID<<"\n"<<Answers[i].FromID<<"\n";
        }
    }
    void UploadAnsweredQuestions()
    {
        ofstream Ans_Ques("AnsweredQuestions.txt",ios::trunc);
        for(int i=0;i<VAnsweredQuestions.size();i++){
            Ans_Ques<<VAnsweredQuestions[i].q<<"\n"<<VAnsweredQuestions[i].r<<"\n";
        }
    }
    void show()
    {
       for(int i=0;i<VAnsweredQuestions.size();i++){
            cout<<VAnsweredQuestions[i].q<<"\n"<<VAnsweredQuestions[i].r<<"\n";
        }
    }
    ~DataBase()
    {
        UploadUsers();
        UploadQuestions();
        UploadAnswers();
        UploadAnsweredQuestions();
    }
};
class Services : public DataBase
{
public:
    virtual void Ask()=0;
    virtual void Respond()=0;
    virtual void ListUsers()=0;
    virtual void ViewMyHistory()=0;
    virtual void View_Questions_I_Asked()=0;
    virtual void View_Forwarded_Questions()=0;
    virtual void Delete()=0;
};
class Account:public Services
{
protected:
    int Command;
    user User;
    vector<QA> MyFeed;
    vector<QA> My_Questions;
    map <string,string> MyAnsweredQuestions;
public:
    Account(){}
    void SignUp()
    {
            cout<<"Enter your name\n" ;
            cin>>User.Name;
            cout<<"Enter your Email address \n";
            cin>>User.Email;
            while(!Validate_Email())
            {
                cout<<"Enter A VALID Email address \n";
                cin>>User.Email;
            }
            cout<<"Set a unique username \n";
            cin>>User.Username;
            while(!Validate_Username())
            {
                cout<<"this usernaem has already been taken,set anotherone\n";
                getline(cin,User.Username);
            }
            cout<<"Set your account password \n";
            cin>>User.Password;
            if(Users_Data.empty()){
                User.ID=1;
            }
            else{
                User.ID = Users_Data[Users_Data.size()-1].ID +1;
            }
            Users_Data.pb(User);
        }
    void SignIn()
    {
        cout<<"Enter your username\n";
        string pass;
        cin>>User.Username;
        cout<<"Enter your password\n";
        cin>>User.Password;
        for(int i=0;i<Users_Data.size();i++){
            if(User.Username==Users_Data[i].Username){
                pass=Users_Data[i].Password;
                User.Name=Users_Data[i].Name;
                User.Email=Users_Data[i].Email;
                User.ID=Users_Data[i].ID;
                break;
            }
        }
        while(pass!=User.Password){
            cout<<"Incorrect password \n";
            cin>>User.Password;
        }
    }
    bool Validate_Email()
    {
        string email = User.Email;
        bool isad=0;
        int idxad=0;
        for(int i=0;i<email.size();i++)
        {
            if(email[i]=='@' && i>0 && i<email.size()-5){
                isad=1;
                idxad=i;
                break;
            }
        }
        if(isad && idxad)
        {
            if(email[email.size()-1]=='m' && email[email.size()-2]=='o' & email[email.size()-3]=='c' && email[email.size()-4]=='.')
            {
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
    bool Validate_Username()
    {
        string uname = User.Username;
        for(int i=0;i<Users_Data.size();i++){
            if(Users_Data[i].Username==uname){
                return 0;
            }
        }
        return 1;
    }
    void StoreMyFeed()
    {
        for(int i=0;i<Questions.size();i++){
            if(Questions[i].ToID==User.ID ){
                string q = Questions[i].qa;
                if(History.find(q)==History.end()){
                     MyFeed.pb(Questions[i]);
                }
            }
        }
    }
    void StoreMyQuestions()
    {
        for(int i=0;i<Questions.size();i++){
            if(Questions[i].FromID==User.ID){
                My_Questions.pb(Questions[i]);
            }
        }
    }
    void ListUsers()
    {
        for(int i=0;i<Users_Data.size();i++){
                if(Users_Data[i].ID!=User.ID){
                    cout<<"username : "<<Users_Data[i].Username<<" ID : "<<Users_Data[i].ID<<"\n";
            }
        }
    }
    void Ask()
    {
        ListUsers();
        cout<<"Enter the user's ID you want to ask\n";
        int To;
        cin>>To;
        cout<<"Enter your question\n";
        string Question;
        char c='?';
        getline(cin,Question,c);
        QA q;
        q.qa=Question;q.FromID=User.ID;q.ToID=To;
        Questions.pb(q);
        UploadQuestions();
        My_Questions.pb(q);
    }
    void Respond()
    {
        StoreMyFeed();
        if(MyFeed.size()){
            full QRes;
            cout<<"enter the question's number from the following\n";
            int Question_Number;
            for(int i=0;i<MyFeed.size();i++){
                cout<<"Question number : "<<i+1<<" "<<MyFeed[i].qa<<" asked by user "<<MyFeed[i].FromID<<"\n";
             }
             cout<<"Enter Question's number : ";cin>>Question_Number;Question_Number--;
             int recepientID = MyFeed[Question_Number].FromID;
             QRes.q=MyFeed[Question_Number].qa;
             cout<<"Respond\n";
             string res;
             char c='.';
             getline(cin,res,c);
             QRes.r=res;
             QA ans;ans.qa=res;ans.ToID=recepientID;ans.FromID=User.ID;
             string fromID = to_string(recepientID);
             string q = MyFeed[Question_Number].qa;
             History[q]=res;
             Answers.pb(ans);
             UploadAnswers();
             VAnsweredQuestions.pb(QRes);
             UploadAnsweredQuestions();
        }
        else{
            cout<<"You have no unanswered forwarded questions\n";
        }
    }
        void ViewMyHistory(){
            map<string,string>::iterator it;
            for(it=History.begin();it!=History.end();++it){
                string question = it->first;
                cout<<"Question : "<<question<<"\nMy answer : "<<History[question];
            }
        }
    void View_Questions_I_Asked(){
        for(int i=0;i<Questions.size();i++)
        {
            if(Questions[i].FromID==User.ID){
                cout<<"Question : "<<Questions[i].qa<<" sent to user "<<Questions[i].ToID<<"\n";
                string q = Questions[i].qa;
                if(History.find(q)!= History.end()){
                    cout<<"Answer : "<<History[q]<<"\n";
                }
                else{
                    cout<<"Unanswered \n";
                }
            }
        }
    }
    void View_Forwarded_Questions(){
        int cnt=0;
         for(int i=0;i<Questions.size();i++){
            if(Questions[i].ToID==User.ID){
                cnt++;
            }
        }
        if(cnt==0){
            cout<<"you don't have any forwarded questions\n";
        }
        else{
          for(int i=0;i<Questions.size();i++){
            if(Questions[i].ToID==User.ID){
                cout<<"Question : "<<Questions[i].qa<<"? From user "<<Questions[i].FromID<<"\n";
                string q = Questions[i].qa;
                if(History.find(q)!=History.end()){
                    cout<<"Answer : "<<History[q]<<"\n";
                }
                else{
                    cout<<"Unanswered!\n";
                }
            }
        }
        }
    }
    void Delete(){
        cout<<"Print the number of the question you want to delete\n";
        int idx,idx2;
        vector<QA>::iterator it;
        for(int i=0;i<Questions.size();i++)
        {
            if(Questions[i].FromID==User.ID){
                cout<<"Question number : "<<i<<" : "<<Questions[i].qa<<"\n";
                string q = Questions[i].qa;
                if(History.find(q)!= History.end()){
                    cout<<"Answer : "<<History[q]<<" by user : "<<Questions[i].ToID<<"\n";
                }
                else{
                    cout<<"Unanswered!\n";
                }
            }
        }
        cin>>idx;
        string qu = Questions[idx].qa;
        for(int i=0;i<VAnsweredQuestions.size();i++){
            if(VAnsweredQuestions[i].q==qu){
                VAnsweredQuestions.erase(VAnsweredQuestions.begin()+i);
                break;
            }
        }
        History.erase(qu);
        Questions.erase(Questions.begin()+idx);
        UploadQuestions();
        UploadAnswers();
        UploadAnsweredQuestions();
    }
};
int main()
{
   Account user1;
   int com;
   cout<<"Enter 1 to sign up\n2 to sign in\n";
   cin>>com;
   if(com==1)
   {
       user1.SignUp();
   }
   else{
    user1.SignIn();
   }
  cout<<"Enter 1 to view Questions forwarded to you \n2 to view questions you asked \n3 to ask a new question \n4 to respond to a question \n5 to list all the users in the system";
  cout<<"\n6 to delete a question\n7 to log out of your account \nand 0 to terminate the program \n";
   while(1)
   {
       cin>>com;
       if(com==1){
        user1.View_Forwarded_Questions();
       }
       else if(com==2){
        user1.View_Questions_I_Asked();
       }
       else if(com==3){
        user1.Ask();
       }
       else if(com==4){
        user1.Respond();
       }
       else if(com==5){
        user1.ListUsers();
       }
       else if(com==6){
        user1.Delete();
       }
       else if(com==7){
        cout<<"logged out successfully!\nEnter 1 to sign up\n2 to sign in\n3 to end the program\n";
        cin>>com;
        if(com==1){
          user1.SignUp();
        }
        else if(com==2){
        user1.SignIn();
        }
        else{
            break;
        }
       cout<<"Enter 1 to view Questions forwarded to you \n2 to view questions you asked \n3 to ask a new question \n4 to respond to a question \n5 to list all the users in the system";
       cout<<"\n6 to delete a question \n7 to log out of your account\nand 0 to terminate the program \n";
       }
       else if(com==0){
           break;
       }
   }
}
