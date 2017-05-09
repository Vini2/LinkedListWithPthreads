#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

struct timeval start, stop;

// Structure of linked list node
typedef struct NODE{
    int Value;
    struct NODE* Next;
};
 
long thread_count;
pthread_mutex_t mutex;
struct NODE *root = NULL;

int n;            // Number of initial unique values in the linked list
int m;            // Number of random Member, Insert, and Delete operations performed on the linked list
float mMember;    // Fraction of Member operations
float mInsert;    // Fraction of Insert operations
float mDelete;    // Fraction of Delete operations
int member_count;
int insert_count;
int delete_count;

// Function declarations
int Member(struct  NODE **root, int Val);
int Insert(struct NODE **root, int Val);
int Delete(struct NODE **root, int Val);
void Display(struct NODE  **root);
void ClearList(struct NODE  **root);
void* thread_oparation(void* rank);


// Main function
int main(int argc, char* argv[]){

    printf("-----------------------------------------------------\n");
    printf("Linked list implementation\n");
    printf("Parallel program (based on Pthreads) with one mutex for the entire linked list\n");

    int i=0, itr=0;
    long thread;
    pthread_t* thread_handles;
    int total_iterations = 100;
    double sample_times[total_iterations];
    double total_time = 0, avg_time = 0, sum_of_squares = 0, standard_deviation;

    // Read command line arguments
    thread_count = strtol(argv[1], NULL, 10);      
    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);
     
    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);

    // Iterate for the given number of iterations
    for(itr = 0; itr < total_iterations; itr++) {

        member_count=0;
        insert_count=0;
        delete_count=0;

        double elapsed_time;

	// Add n number of elements to the linked list
        for(i=0; i<n; i++){
            int r = rand()%65536;
            if(!Insert(&root, r)){
              i--;
            }
        }

        // Print elements on the linked list
        //Display(&root);
	
        thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));  
     
        // Get start time	
        gettimeofday(&start, NULL);
	
        pthread_mutex_init(&mutex, NULL);
     
        for (thread = 0; thread < thread_count; thread++)  {
            pthread_create(&thread_handles[thread], NULL,thread_oparation , (void*)thread);  
        }
	     
        for (thread = 0; thread < thread_count; thread++) {
            pthread_join(thread_handles[thread], NULL); 
        }
     
        pthread_mutex_destroy(&mutex);

        // Get end time	
        gettimeofday(&stop, NULL);

        // Calculate elapsed time	
        elapsed_time = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    	
        // Assign sample time
        sample_times[itr] = elapsed_time; 

        // Add to total time
        total_time += elapsed_time;
	   
	   // Clear list and free memory
	   ClearList(&root);

    }

    // Calculate mean time 
    avg_time = total_time / total_iterations;

    // Calculate standard deviation of the sample
    for(itr = 0; itr < total_iterations; itr++) {
        sum_of_squares += (avg_time - sample_times[itr]) * (avg_time - sample_times[itr]);
    }
    standard_deviation = sqrt (sum_of_squares/(total_iterations-1));
 
    // Print results
    printf("Results:\n");
    printf("\tTotal elapsed time: %f s\n", total_time);
    printf("\tTotal iterations: %d\n", total_iterations);
    printf("\tAverage time: %f s\n", avg_time);
    printf("\tStandard deviation: %f\n", standard_deviation);
    printf("-----------------------------------------------------\n");

    return 0;
}

void* thread_oparation(void* rank) 
{
    long long i;
    long long avg_m = m/thread_count;
    for( i=0; i < avg_m; i++ )
    {
 
        float prob = (rand()%10000/10000.0);
     
        int r = rand()%65536;
        if(prob<mMember)
        {
            pthread_mutex_lock(&mutex);
            Member(&root, r);
            member_count++;
            pthread_mutex_unlock(&mutex);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_mutex_lock(&mutex);
            Insert(&root, r);
            insert_count++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {           
            pthread_mutex_lock(&mutex);
            Delete(&root, r);
            delete_count++;
            pthread_mutex_unlock(&mutex);
        }   
    }  
 
   return NULL;
}

int Member(struct  NODE **root, int Val){
    struct NODE *curr_p = *root;
    
    // Traverse till element is found
    while(curr_p != NULL && curr_p->Value < Val){
        curr_p = curr_p->Next;
    }
 
    if(curr_p == NULL || curr_p->Value > Val){
        //printf("%d does not exist\n", Val);
        return 0;
    } else {
        //printf("%d exists\n", Val);
        return 1;
    }
}

int Insert(struct NODE **root, int Val) {
    struct NODE* curr_p = *root;          
    struct NODE* pred_p = NULL;
    struct NODE* temp_p = NULL;
 
    while(curr_p !=NULL && curr_p->Value < Val)
    {
        pred_p = curr_p;
        curr_p = curr_p->Next;
    }
     
    if(curr_p == NULL || curr_p->Value > Val)
    {
        temp_p = malloc(sizeof(struct NODE));        
        temp_p->Value = Val;
        temp_p->Next = curr_p;
         
        if(pred_p == NULL)
        {
            *root = temp_p;
        }
        else
        {
            pred_p->Next = temp_p;
        }
        return 1;
  
    }
    else
    {
        return 0;
    }
}

int Delete(struct NODE **root, int Val) {

    struct NODE* curr_p = *root;
    struct NODE* pred_p = NULL;
     
    while(curr_p != NULL && curr_p->Value < Val){
        pred_p = curr_p;
        curr_p = curr_p->Next;
    }   
     
    if(curr_p != NULL && curr_p->Value < Val){
        if(pred_p == NULL){
            *root = curr_p->Next;
            free(curr_p);
        } else{
            pred_p->Next = curr_p->Next;
            free(curr_p);
        }
        return 1;
         
    }else{
        return 0;
    }
}

void Display(struct NODE  **root) {
    struct NODE *p = *root;

    printf("Values in linked list: ");

    // Print the values in the linked list
    while(p != NULL){
        printf("%d ", p->Value);
        p = p->Next;
    }

    printf("\n");

}

void ClearList(struct NODE  **root){
    struct NODE *curr_p;
    struct NODE *succ_p;

    if (*root == NULL) {
        return;
    }

    curr_p = *root;
    succ_p = curr_p->Next;

    while (succ_p != NULL) {
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->Next;
    }

    free(curr_p);
    *root = NULL;

}

