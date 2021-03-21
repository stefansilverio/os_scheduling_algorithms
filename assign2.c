#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

struct node {
   char str[25];
   int num;
   struct node* next; 
};

void push(struct node **head_ref, char *str, int num);
void run(struct node **head_ref);
void requeue(struct node **head_ref, struct node *prev);
void delete_node(struct node **head_ref, struct node **prev_ptr);
void print_list(struct node **head_ref);
void swap(struct node **prev_ptr, struct node **curr_ptr);

void my_handle(int signal){};

int main()
{
   struct node* start_ptr = NULL;
   int num;
   char str[25];
   FILE *fptr;

   if ((fptr = fopen("/home/silver/c-experiments/process_list2.txt", "r")) == NULL)
   {
      printf("Error opening files\n");
      exit(1);
   }

   while (fscanf(fptr, " %s %d", str, &num) != EOF)
      push(&start_ptr, str, num);

   fclose(fptr);

   printf("This is what your linked list of processes looks like at the beginning:\n\n");
   print_list(&start_ptr);

   printf("Running tasks:\n\n");
   while (start_ptr != NULL)
   {
      run(&start_ptr);
      printf("Cycling through process queue again:\n");
   }

   printf("All processes complete!!\n\n");

   return 0;
}

// This function builds queue
void push(struct node** head_ref, char *str, int num)
{
   struct node* new_node = (struct node*)malloc(sizeof(struct node));

   if (new_node == NULL)
   {
      printf("Malloc failed\n");
      exit(1);
   }
   
   strcpy(new_node->str, str); // so that we're not saving same ptr in struct everytime
   new_node->num = num;

   new_node->next = (*head_ref);
   (*head_ref) = new_node;
}

// Calls either requeue or delete on process
void run(struct node **head_ref)
{
   struct node *curr_ptr = *head_ref;
   struct node *prev_ptr = NULL;
   int interrupt_flag = -1;
   int increment_flag = -1;

   signal(SIGALRM, my_handle);

   while (curr_ptr)
   {
      printf("%s %d\n", curr_ptr->str, curr_ptr->num);
      increment_flag = -1;
      alarm(3);
      interrupt_flag = sleep(curr_ptr->num);
      if (interrupt_flag != 0) // if process needs didn't complete and needs requeue()
      {
         curr_ptr->num = interrupt_flag;
         if (prev_ptr != NULL)
         {
            requeue(head_ref, prev_ptr);
            swap(&prev_ptr, &curr_ptr);
         }
      }

      // delete_node() function will handle increment
      else 
      {
         increment_flag = 0;

         printf("%s Finished\n", curr_ptr->str);
         curr_ptr = curr_ptr->next; // save curr_ptr - curr_ptr could be at end of list
         delete_node(head_ref, &prev_ptr);
      }

      // if delete_node() function didn't get called we need to handle increment
      if (increment_flag != 0)
      {
         prev_ptr = curr_ptr;
         curr_ptr = curr_ptr->next;
      }
   }
}

// moves process back to front of queue
void requeue(struct node **head_ref, struct node *prev_ptr)
{
   // requeue last node in list 
   if (prev_ptr->next->next == NULL)
   {
      prev_ptr->next->next = (*head_ref);
      *head_ref = prev_ptr->next;
      prev_ptr->next = NULL;
   }

   // general case for middle of list
   else
   {
      struct node *tmp_ptr = prev_ptr->next;

      prev_ptr->next = tmp_ptr->next;
      tmp_ptr->next = (*head_ref);
      *head_ref = tmp_ptr;
   }
}

void swap(struct node **prev_ptr, struct node **curr_ptr)
{
   struct node *tmp_ptr = *curr_ptr;

   *curr_ptr = *prev_ptr;
   *prev_ptr = tmp_ptr;
}

void delete_node(struct node **head_ref, struct node **prev_ptr)
{
   //delete first node in list 
   if ((*prev_ptr) == NULL)
   {
      struct node *tmp_ptr = *head_ref;
      *head_ref = tmp_ptr->next;
      free(tmp_ptr);
   }

   // last node in list case
   else if ((*prev_ptr)->next->next == NULL)
   {
      free((*prev_ptr)->next);
      (*prev_ptr)->next = NULL;
   }

   // normal (middle case)
   else
   {
      struct node *tmp_ptr = (*prev_ptr)->next;
      (*prev_ptr)->next = tmp_ptr->next;
      free(tmp_ptr);
   }
}

void print_list(struct node **head_ref)
{
   struct node *curr_ptr = (*head_ref);

   while (curr_ptr)
   {
      printf("%s %d\n", curr_ptr->str, curr_ptr->num);
      curr_ptr = curr_ptr->next;
   }
   printf("\n");
}

