#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTRLN 51


// Code written by Mohamad Mustafa on 12/08/22


typedef struct Date
{
  int day;
  int month;
  int year;
} Date;

typedef struct Student
{
  int id;
  char* fName; 
  char* lName; 
  Date bday;
} Student;

typedef struct Course
{
  char id[MAXSTRLN];
  char name[MAXSTRLN];
  float credit;
} Course;

typedef struct Enrollment
{
  int studentID;
  char courseID[MAXSTRLN];
  char semester[MAXSTRLN];
  float score;
} Enrollment;


char studentfile[31], coursefile[31], enrollmentfile[31];
Student* loadStudents(int* numStudents);
Enrollment* loadEnrollment(int *numEnrollments);
Course* loadCourses(int *numCourses);
void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
char getGrade(float key);
void printStudentInfo(Enrollment* ArrayOfEnrollments, int amountEnrolls, Course* ArrayOfCourses, int amountCourses, int studentID);
void searchStudents(Student *ArrayOfStudents, int amountStudents, Enrollment *ArrayOfEnrollments, int amountEnrolls, Course* ArrayOfCourses, int amountCourses);
void searchCourses(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
Course* addCourse(Course *ArrayOfCourses, int *amountCourses);
void count(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls);
void sort(Student* ArrayOfStudents, int amountStudents);
void freeMem(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, Enrollment *ArrayOfEnrollments);



int main() {

  int numStudents;
  int numEnrollments;
  int numCourses;

  // takes input of file names from user
  scanf("%s %s %s", studentfile, coursefile, enrollmentfile);

  // loads student, enrollment and course arrays from each function
  Student* stdArr = loadStudents(&numStudents);
  Enrollment* enrollmentArr = loadEnrollment(&numEnrollments);
  Course* courseArr = loadCourses(&numCourses);

  // calls menu function for the rest of the inputs
  menu(stdArr, numStudents, courseArr, numCourses, enrollmentArr, numEnrollments);

  // passes in dynamically allocated arrays and frees the memory with freeMem function
  freeMem(stdArr, numStudents, courseArr, enrollmentArr);
  
  return 0;
}



void menu(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls) {

  char command[MAXSTRLN];
  
  printf("==================\n");

  // initial command to begin loop
  scanf("%s", command);

  
  // infinite menu loop, uses strcmp function to determine which function to run based on user inputted command, if command is "exit", breaks loop by returning to main
  while(1)
  {
    
    if(!(strcmp(command, "search_students")))
    {
      searchStudents(ArrayOfStudents, amountStudents, ArrayOfEnrollments, amountEnrolls, ArrayOfCourses, amountCourses);
    }
    else if(!(strcmp(command, "search_course")))
    {
      searchCourses(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    }
    else if(!(strcmp(command, "add_course")))
    {
      ArrayOfCourses = addCourse(ArrayOfCourses, &amountCourses);
    }
    else if(!(strcmp(command, "count")))
    {
      count(ArrayOfStudents, amountStudents, ArrayOfCourses, amountCourses, ArrayOfEnrollments, amountEnrolls);
    }
    else if(!(strcmp(command, "sort")))
    {
      sort(ArrayOfStudents, amountStudents);
    }
    else if(!(strcmp(command, "exit")))
    {
      printf("------------------\nBye!");
      return;
    }
    printf("==================\n");
    scanf("%s", command);
  }
  
}



Student* loadStudents(int* numStudents) {

  int id;
  char fname[MAXSTRLN];
  char lname[MAXSTRLN];
  Date birth;

  // opens student file and extracts the number of students from first line
  FILE* inStudent = fopen(studentfile, "r");
  fscanf(inStudent, "%d", numStudents);

  // uses malloc to allocated memory for an array of students depending on the number of students
  Student* ArrayOfStudents = malloc(*numStudents * sizeof(Student));

  // loop scans in the information from file into temp variables into array, and allocates memory for first and last names by finding the length of the name then allocating the exact amount of memory based on that
  for (int i=0; i < *numStudents; i++)
  {
    fscanf(inStudent, "%d %s %s %d %d %d", &id, fname, lname, &birth.day, &birth.month, &birth.year);

    int fnameLength = strlen(fname) + 1;
    int lnameLength = strlen(lname) + 1;

    ArrayOfStudents[i].fName = malloc(fnameLength * (sizeof(char)));
    ArrayOfStudents[i].lName = malloc(lnameLength * (sizeof(char)));

    strcpy(ArrayOfStudents[i].fName, fname);
    strcpy(ArrayOfStudents[i].lName, lname);

    ArrayOfStudents[i].id = id;
    ArrayOfStudents[i].bday.day = birth.day;
    ArrayOfStudents[i].bday.month = birth.month;
    ArrayOfStudents[i].bday.year = birth.year;
  }

  fclose(inStudent);

  // returns the loaded student array
  return ArrayOfStudents;
}



Enrollment* loadEnrollment(int *numEnrollments) {

  char courseId[MAXSTRLN];
  char semester[MAXSTRLN];
  int id;
  float score;

  // same logic from student array used here to opne file and allocate memory for an enrollment array
  FILE* inEnroll = fopen(enrollmentfile, "r");
  fscanf(inEnroll, "%d", numEnrollments);

  Enrollment* ArrayOfEnrollments = malloc(*numEnrollments * sizeof(Enrollment));

  // loops through file based on num enrollments and loads data into the array
  for(int i=0; i < *numEnrollments; i++)
  {
    fscanf(inEnroll, "%d %s %s %f", &id, courseId, semester, &score);

    strcpy(ArrayOfEnrollments[i].courseID, courseId);
    strcpy(ArrayOfEnrollments[i].semester, semester);

    ArrayOfEnrollments[i].studentID = id;
    ArrayOfEnrollments[i].score = score;
  }

  fclose(inEnroll);

  // returns loaded enrollment array
  return ArrayOfEnrollments;
}



Course* loadCourses(int *numCourses) {

  char id[MAXSTRLN];
  char name[MAXSTRLN];
  float credits;

  // same logic used here as student and course array
  FILE* inCourse = fopen(coursefile, "r");
  fscanf(inCourse, "%d", numCourses);

  Course* ArrayOfCourses = malloc(*numCourses * sizeof(Course));

  // loops through file based on num courses and loads data into the array
  for (int i=0; i < *numCourses; i++)
  {
    fscanf(inCourse, "%s %s %f", id, name, &credits);

    strcpy(ArrayOfCourses[i].id, id);
    strcpy(ArrayOfCourses[i].name, name);

    ArrayOfCourses[i].credit = credits;
  }

  fclose(inCourse);

  // returns loaded course array
  return ArrayOfCourses;
}



char getGrade(float key) {

  // uses if else ladder to determine grade and returns char of grade
  if (key >= 90.00)
    return 'A';
  else if (key >= 80.00 && key < 90.00)
    return 'B';
  else if (key >= 70.00 && key < 80.00)
    return 'C';
  else if (key >= 60.00 && key < 70.00)
    return 'D';
  else
    return 'F';
  
}



void printStudentInfo(Enrollment* ArrayOfEnrollments, int amountEnrolls, Course* ArrayOfCourses, int amountCourses, int studentID) {

  // loops through each student ID in enrollments and if it matches the passed studentID , then cycles through courses to find the correct course id, then uses getGrade function to retrieve the grade of the course and prints the courseid, name, semester, score, and grade

  for(int j=0; j < amountEnrolls; j++)
  {
    if (ArrayOfEnrollments[j].studentID == studentID)
    {
      for(int i=0; i < amountCourses; i++)
      {
        if (!strcmp(ArrayOfCourses[i].id, ArrayOfEnrollments[j].courseID))
        {
          char grade = getGrade(ArrayOfEnrollments[j].score);
        
          printf("%s %s %s %.0f %c\n", ArrayOfEnrollments[j].courseID, ArrayOfCourses[i].name, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
        }
      }
     }
   }

  
}



void searchStudents(Student *ArrayOfStudents, int amountStudents, Enrollment *ArrayOfEnrollments, int amountEnrolls, Course* ArrayOfCourses, int amountCourses) {

  // takes a subcommand input from user
  char subcommand[MAXSTRLN];
  scanf("%s", subcommand);

  // if else ladder determines which logic to run based on inputed subcommand
  if(!(strcmp(subcommand, "lname")))
  {

    // scans in the desired lname from user and declares a flag int
    char key[MAXSTRLN];
    int flag = 0;
    scanf("%s", key);

    // cycles through student array to find the last name, if found prints the information of the student and changes flag = 1
    for (int i=0; i < amountStudents; i++)
    {
      
      if (!(strcmp(key, ArrayOfStudents[i].lName)))
      {
        
        printf("------------------\n");
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].bday.month, ArrayOfStudents[i].bday.day, ArrayOfStudents[i].bday.year);
        printStudentInfo(ArrayOfEnrollments, amountEnrolls, ArrayOfCourses, amountCourses, ArrayOfStudents[i].id); 
        
        flag = 1;
      }
      
    }
    
    if (!flag)
      printf("------------------\nnot found\n");
    printf("------------------\n");
  }
    
  else if(!(strcmp(subcommand, "id")))
  {
    
    // scans in int key for desired id to be found
    int key;
    int flag = 0;
    scanf("%d", &key);

    // loops through student array to find the id that matches, and if so prints the student info and changes flag = 1
    for(int i=0; i < amountStudents; i++)
    {
      
      if (key == ArrayOfStudents[i].id)
      {
        
        printf("------------------\n");
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].bday.month, ArrayOfStudents[i].bday.day, ArrayOfStudents[i].bday.year);
        printStudentInfo(ArrayOfEnrollments, amountEnrolls, ArrayOfCourses, amountCourses, ArrayOfStudents[i].id);   
        
        flag = 1;
      }
      
    }
    
    if (!flag)
      printf("------------------\nnot found\n");
    printf("------------------\n");
  }

  else if(!(strcmp(subcommand, "byear")))
  {
    
    // takes input of year using int key
    int key;
    int flag = 0;
    scanf("%d", &key);

    //loops through student enrollment to find the matching year and prints the info of that student and changes flag = 1
    for(int i=0; i < amountStudents; i++)
    {
      
      if(ArrayOfStudents[i].bday.year == key)
      {
        
        printf("------------------\n");
        printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].bday.month, ArrayOfStudents[i].bday.day, ArrayOfStudents[i].bday.year);
        printStudentInfo(ArrayOfEnrollments, amountEnrolls, ArrayOfCourses, amountCourses, ArrayOfStudents[i].id);
        
        flag = 1;
      }
      
    }
    if (!flag)
      printf("------------------\nnot found\n");
    printf("------------------\n");
  }

  
}



void searchCourses(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls) {

  // takes input for subcommand
  char subcommand[MAXSTRLN];
  scanf("%s", subcommand);


  // if else ladder determines which logic to run based on inputed subcommand
  if(!(strcmp(subcommand, "cid")))
  {
    
    // takes input for desired cid string
    char key[MAXSTRLN];
    int flag = 0;
    scanf("%s", key);

    // loops through amount of courses to find matching cid 
    for (int i=0; i < amountCourses; i++)
    {

      // changes flag = 1 then finds the course in enrollment and prints the information of all students with that course from enrollment by looping through student arrray
      if(!(strcmp(key, ArrayOfCourses[i].id)))
      {
        flag = 1;
        printf("------------------\n");
        printf("%s %.0f\n", ArrayOfCourses[i].name, ArrayOfCourses[i].credit);
        
        for(int j=0; j < amountEnrolls; j++)
        {
          if(!(strcmp(ArrayOfCourses[i].id, ArrayOfEnrollments[j].courseID)))
          {
            for(int k=0; k < amountStudents; k++)
            {
              if(ArrayOfStudents[k].id == ArrayOfEnrollments[j].studentID)
              {
                char grade = getGrade(ArrayOfEnrollments[j].score);
                printf("%s %s %s %.0f %c\n", ArrayOfStudents[k].fName, ArrayOfStudents[k].lName, ArrayOfEnrollments[j].semester, ArrayOfEnrollments[j].score, grade);
              }
            }
          }
        }
      }
      
    }

    if (!flag) 
      printf("------------------\nnot found\n");
    printf("------------------\n");
  }


  else if(!(strcmp(subcommand, "cid_semester")))
  {
    char courseKey[MAXSTRLN];
    char semesterKey[MAXSTRLN];
    int flag = 0;

    // takes input for desired semester and course to search
    scanf("%s", courseKey);
    scanf("%s", semesterKey);
    
    printf("------------------\n");

    // loops through enrollment to find a matching course id and semester 
    for(int i=0; i < amountEnrolls; i++)
    {
      if(!(strcmp(ArrayOfEnrollments[i].courseID, courseKey)) && !(strcmp(ArrayOfEnrollments[i].semester, semesterKey)))
      {

        // loops through the courses to find a match between the ids then prints the name of the course and credit
        for(int j=0; j < amountCourses; j++)
        {
          if(!(strcmp(ArrayOfCourses[j].id, ArrayOfEnrollments[i].courseID)) && !flag)
          {
            flag = 1;
            printf("%s %.0f\n", ArrayOfCourses[j].name, ArrayOfCourses[j].credit);
            break;
          }
        }


        // loops through the students and prints student info if they have that course in that semester
        for(int k=0; k < amountStudents; k++)
        {
          if(ArrayOfEnrollments[i].studentID == ArrayOfStudents[k].id)
          {
            char grade = getGrade(ArrayOfEnrollments[i].score);
            printf("%s %s %s %.0f %c\n", ArrayOfStudents[k].fName, ArrayOfStudents[k].lName, ArrayOfEnrollments[i].semester, ArrayOfEnrollments[i].score, grade);
          }
        }

        
      }
    }
    
    if (!flag) 
      printf("not found\n");
    printf("------------------\n");
  }
  
}



Course* addCourse(Course *ArrayOfCourses, int *amountCourses) {

  float credit;
  char id[MAXSTRLN];
  char name[MAXSTRLN]; 

  // takes user input for temporary variables of the course info
  scanf("%s", id);
  scanf("%s", name);
  scanf("%f", &credit);

  // loop compares the id of the inputed course with every existing course, if it matches any course it prints course violation and returns 
  printf("------------------\n");
  for(int i=0; i < *amountCourses; i++)
  {
    if (!(strcmp(id, ArrayOfCourses[i].id)))
    {
      printf("violation of course id uniqueness. Cannot add the record.\n------------------\n");
      return ArrayOfCourses;
    }
  }

  // Opens the course file in r+ mode for reading and writing
  FILE* f_Course = fopen(coursefile, "r+");

  // relplaces amount of courses in file with updated count
  fprintf(f_Course, "%d\n", *amountCourses + 1);

  // frees the original ArrayOfCourses memory and calls loadCourses to create a new array with the correct amount of memory allocated based on the new course count
  free(ArrayOfCourses);
  Course* ArrayOfCoursesNew = loadCourses(amountCourses);

  // updates amountCoirses pointer 
  *amountCourses += 1;

  // loads the new course array with the user inputed new course info
  ArrayOfCoursesNew[*amountCourses-1].credit = credit;
  strcpy(ArrayOfCoursesNew[*amountCourses-1].id, id);
  strcpy(ArrayOfCoursesNew[*amountCourses-1].name, name);

  // moves file pointer to end of file
  fseek(f_Course, 0, SEEK_END);

  // prints the new course informatoon at the end of the course file
  fprintf(f_Course, "\n%s %s %.0f", ArrayOfCoursesNew[*amountCourses-1].id, ArrayOfCoursesNew[*amountCourses-1].name, ArrayOfCoursesNew[*amountCourses-1].credit);
  
  fclose(f_Course);

  printf("record added\n------------------\n");

  // returns updated course array
  return ArrayOfCoursesNew;
}



void count(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, int amountCourses, Enrollment *ArrayOfEnrollments, int amountEnrolls) {

  char subCommand[MAXSTRLN];
  scanf("%s", subCommand);

  printf("------------------\n");

  // if else ladder determines which logic to run based on subcommand input by user
  if(!(strcmp(subCommand, "students")))
  {

    // prints the current student total
    printf("total students %d\n", amountStudents);
    printf("------------------\n");
    return;
    
  }

  else if(!(strcmp(subCommand, "students_course")))
  {

    // loops through the enrollment array for each course, if course id matches adds, 1 to the count for that course then prints the course and the count
    for(int i=0; i < amountCourses; i++)
    {
      int count = 0;
      for (int j=0; j < amountEnrolls; j++)
      {
        if(!(strcmp(ArrayOfCourses[i].id, ArrayOfEnrollments[j].courseID)))
          count++;
      }
      printf("%s %d\n", ArrayOfCourses[i].id, count);
    }
    printf("------------------\n");
    return;
  }

  else if(!(strcmp(subCommand, "students_semester")))
  {
    
    // declares a semester list 2d string array of max size 100, a student count array and a semester count array
    char semList[100][MAXSTRLN];
    int studentCount[100];
    int countSem = 0;

    // initializes all numbers in student count array as 0
    for (int i=0; i < 100; i++)
      studentCount[i] = 0;

    
    // loops through the semester list to check if that semester i on the enrollment array has alreafy been added, if so, adds 1 to the corresponding count in student array, changes flag = 1 and breaks inner loop
    for (int i=0; i < amountEnrolls; i++)
    {
      
      int flag = 0;

      for (int j=0; j < countSem; j++)
      {
        if(!(strcmp(ArrayOfEnrollments[i].semester, semList[j])))
        {
          studentCount[j] += 1;
          flag = 1;
          break;
        }
      }

      // if the semester for that enrollment array i not found in semester list, adds the semester to the list, adds one to corresponding student count and one to semester count
      if(!flag)
      {
        strcpy(semList[countSem], ArrayOfEnrollments[i].semester);
        studentCount[countSem] += 1;
        countSem += 1;
      }
      
    }

    // prints the semesters and the count per semester
    for(int i=0; i < countSem; i++)
      printf("%s %d\n", semList[i], studentCount[i]);
    printf("------------------\n");
  }
  
}



void sort(Student* ArrayOfStudents, int amountStudents) {

  // bubble sort logic to sort students in array in increasing order using temp student variable and swapping
  for (int i=0; i < amountStudents - 1; i++)
  {
    for(int j=0; j < amountStudents - i - 1; j++)
    {
      if(ArrayOfStudents[j].id > ArrayOfStudents[j + 1].id)
      {
        // swap student
        Student temp = ArrayOfStudents[j];
        ArrayOfStudents[j] = ArrayOfStudents[j + 1];
        ArrayOfStudents[j + 1] = temp;
      }
    }
  }

  printf("------------------\n");

  // prints the information of each student in array
  for(int i=0; i < amountStudents; i++)
    printf("%d %s %s %d/%d/%d\n", ArrayOfStudents[i].id, ArrayOfStudents[i].fName, ArrayOfStudents[i].lName, ArrayOfStudents[i].bday.month, ArrayOfStudents[i].bday.day, ArrayOfStudents[i].bday.year);

  printf("------------------\n");
  
}



void freeMem(Student *ArrayOfStudents, int amountStudents, Course *ArrayOfCourses, Enrollment *ArrayOfEnrollments) {

  // loop to free the memory of each allocated string the the student structures
  for (int i=0; i < amountStudents; i++)
  {
    free(ArrayOfStudents[i].fName);
    free(ArrayOfStudents[i].lName);
  }

  // frees the pointers of each array
  free(ArrayOfStudents);
  free(ArrayOfCourses);
  free(ArrayOfEnrollments);
}