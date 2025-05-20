#include<bits/stdc++.h>
using namespace std;

class student{
    public:
        string name;
        int year;
        map<string,int> completedcourse;
        map<string,int> enrolledcourses;
        map<string,int> slots;      //slots of student currently(additional member i added)
    
        student(string na,int ye,map<string,int> ccourses){
            name=na;
            year=ye;
            completedcourse=ccourses;
        }
        student(){
            name="";
            year=0;
        }
        bool find_all(map<string,int> s){
            for(auto itr=s.begin();itr!=s.end();itr++){
                if(completedcourse.find(itr->first)==completedcourse.end()){
                    return false;
                }
            }
            return true;
        }
    friend class course;
};

class course{
    public:
        string name;
        int noc;
        int capacity;
        int nos;
        map<string,int> prere;
        string slot;    
        /*additional members*/
        map<string,int> stud_this;      //students under this course
        queue<string> waitlist;         //waitlist using queue

        course(string na,int no_cr,int cap,string tslot,map<string,int> pre){
            name=na;
            noc=no_cr;
            capacity=cap;
            slot=tslot;
            prere=pre;
            nos=0;
        }
        course(){
            name="";
            noc=0;
            capacity=0;
            slot="";
            nos=0;
        }
        /* for printing of students under this course */
        void print(){
            for(auto itr=stud_this.begin();itr!=stud_this.end();itr++){
                cout <<itr->first<<" ";
            }
            cout <<endl;
        }
    friend class student;
};

/*funtion for checking wheather all conditions for enrolling a student are met or not*/
bool check_for_enroll(string studentid,string courseid,map<string,student> &students,map<string,course> &courses){
    bool a1=true,a2,a4=true;
            if(students.find(studentid)!=students.end() && courses.find(courseid)!=courses.end()){
                ;
            }else{
                a1=false;
            }
            if(students[studentid].slots.find(courses[courseid].slot)!=students[studentid].slots.end()){
                a4=false;
            }
            a2=students[studentid].find_all(courses[courseid].prere);
            return (a1 && a2 && a4);
}

/*waitlist processing*/
void process_waitlist(queue<string> &q,string &courseid,map<string,student> &students,map<string,course> &courses){
    if(!q.empty()){
        string temp=q.front();
        while(!q.empty() && !check_for_enroll(temp,courseid,students,courses)){
            q.pop();
            if(!q.empty()){
                temp=q.front();
            }
        }
        if(!q.empty()){
            string stu=temp;
            students[stu].enrolledcourses[courseid]++;
            students[stu].slots[courses[courseid].slot]++;
            courses[courseid].stud_this[stu]++;
            courses[courseid].nos++;
            q.pop();
        }
    }
}

int main() {
    map<string,student> students;   //list of students
    map<string,course> courses;     //list of courses
    int n;
    cin >>n;
    string s;
    string studentid,name,courseid,sl;
    int year,nocc,nocr,ca,k;
    for(int i=0;i<n;i++){
        cin >>s;
        if(s=="add_student"){
            cin >>studentid>>name>>year>>nocc;
            map<string,int> sample;
            string z;
            for(int j=0;j<nocc;j++){
                cin >>z;
                sample[z]++;
            }
            student s(name,year,sample);
            students[studentid]=s;
        }else if(s=="add_course"){
            bool x=true;
            cin >>courseid>>name>>nocr>>ca>>sl>>k;
            map<string,int> sam;
            string z;
            for(int j=0;j<k;j++){
                cin >>z;
                sam[z]++;
                if(courses.find(z)==courses.end()){
                    x=false;
                }
            }
            if(x){
                course c(name,nocr,ca,sl,sam);
                courses[courseid]=c;
            }
        }else if(s=="enroll"){
            cin >>studentid>>courseid;
            if(courses[courseid].nos>=courses[courseid].capacity){
                courses[courseid].waitlist.push(studentid);
            }else if(check_for_enroll(studentid,courseid,students,courses)){
                students[studentid].enrolledcourses[courseid]++;
                students[studentid].slots[courses[courseid].slot]++;
                courses[courseid].stud_this[studentid]++;
                courses[courseid].nos++;
            }
        }else if(s=="drop"){
            cin >>studentid>>courseid;
            bool b1=true,b2=true;
            if(students.find(studentid)!=students.end() && courses.find(courseid)!=courses.end()){
                ;
            }else{
                b1=false;
            }
            if(courses[courseid].stud_this.find(studentid)==courses[courseid].stud_this.end()){
                b2=false;
            }
            if(b1 && b2){
                students[studentid].enrolledcourses.erase(courseid);
                students[studentid].slots.erase(courses[courseid].slot);
                courses[courseid].stud_this.erase(studentid);
                courses[courseid].nos--;
                process_waitlist(courses[courseid].waitlist,courseid,students,courses);
            }
        }else if(s=="print"){
            cin >>courseid;
            if(courses.find(courseid)!=courses.end()){
                cout<<"Enrolled students in "<<courseid<<":"<<endl;
                courses[courseid].print();
            }else{
                cout<<"Invalid Course"<<" "<<courseid<<endl;
            };
        }
    }
}