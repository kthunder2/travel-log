#include <stdio.h>
#include <stdlib.h>
#include<string.h>

struct tas
{
    int uniqueID;
    char countryName[20];
    char departureDate[20];
    char returnDate[20];
    char travelType[20];
    struct tas *next;

};

struct tas* initializeTravels(char *);
void printTravels(struct tas *);
void addTravel(struct tas *);
void deleteTravel (struct tas *, int);
void searchTravels(struct tas*);
void createSpecialList(struct tas*,char);
void Overwrite(struct tas*);

int main(int argc, char *argv[])
{
    struct tas *aa, *tmp; //tmp for freeing memory

    int selection=0;
    char textfile[20];
    int del;
    char traveltype;

    strcpy(textfile, argv[1]);

    aa=initializeTravels(textfile);
    printTravels(aa);



    while(selection!=6)
    {
        printf("\n-----MENU---------------------------------------\n");
        printf("1. Add Travel\n2. Delete Travel\n3. Print Travels\n4. Search Travels\n5. Create Special List\n6. Exit");
        printf("\nEnter your selection:");
        scanf("%d", &selection);
        fflush(stdin);

        if(selection==1)
        {
            addTravel(aa);
        }
        else if(selection==2)
        {
            printf("\nEnter the ID of the travel you want to delete:");
            scanf("%d", &del);
            fflush(stdin);
            deleteTravel (aa, del);

        }
        else if(selection==3)
        {
            printTravels(aa);
        }
        else if(selection==4)
        {
            searchTravels(aa);
        }
        else if(selection==5)
        {

            printf("Enter travel type (B/H/E):");
            scanf("%c", &traveltype);
            fflush(stdin);
            createSpecialList(aa,traveltype);
        }
        else if(selection==6)
        {
            Overwrite(aa);
            printf("\nThe travelArchive.txt file has been updated successfully!!");
        }
        else
        {
            printf("\nInvalid input, try again!\n");
        }


    }

    //after we're done with the code, we release allocated memory from linked list, since we don't need it anymore.
    while (aa != NULL)
    {
        tmp = aa;
        aa = aa->next;
        free(tmp);
    }



    return 0;
}

struct tas* initializeTravels(char textfile[20])
{
    struct tas *temp, *head=NULL;

    char lenght_string[100]; //just for lenght check purposes
    int lenght;
    int x=1;

    FILE *travel;

    travel=fopen(textfile, "r");

    if(travel==NULL)
    {
        printf("Couldn't open file!");
        exit(1);
    }
    else
    {
        printf("The travelArchive.txt file has been loaded successfully!");
    }

    temp=(struct tas *)malloc(sizeof(struct tas));
    head=temp;
    temp->next=NULL;

    //this loop checks how many lines are in the text file.
    while(fscanf(travel,"%s", lenght_string)!=EOF)
    {
        lenght++;
    }

    rewind(travel); //go back to the start of the file

    while(fscanf(travel,"%d;%[^;];%[^;];%[^;];%s", &(temp->uniqueID), temp->countryName, temp->departureDate, temp->returnDate, temp->travelType)!=EOF && x<lenght)
    {
        temp->next=(struct tas *)malloc(sizeof(struct tas));
        temp=temp->next;
        x++;
    }

    //actually was a headache to find this; code was working but had problems with printing until I added this little thing below.
    //My linked list didn't even had a specified ending point before I added this! :)
    temp->next=NULL;




    fclose(travel);

    return head;
}

void printTravels(struct tas *aa)
{
    struct tas *read;
    read=aa;

    while(read!=NULL)
    {
        printf("\n%d;%s;%s;%s;%s", read->uniqueID,read->countryName,read->departureDate,read->returnDate,read->travelType);
        read=read->next;
    }
}

void addTravel(struct tas *aa)
{
    struct tas *temp, *last;
    temp=aa;
    char type;
    int checker=1;
    int id=2;

    //traverse the list until the end of the linked list
    while(temp->next!=NULL)
    {
        temp=temp->next;
        id++;
    }

    last=temp;

    last->next=(struct tas*)malloc(sizeof(struct tas));
    last=last->next;

    last->uniqueID=id;
    printf("\nEnter the name of the country:");
    scanf("%s", last->countryName);
    fflush(stdin);
    printf("\nEnter departure date:");
    scanf("%s", last->departureDate);
    fflush(stdin);
    printf("\nEnter return date:");
    scanf("%s", last->returnDate);
    fflush(stdin);
    printf("\nEnter travel type (B/H/E):");

    //depending on the travel type, the whole travel type will be written to respective linked list place, with an error check.
    while(checker==1)
    {
        scanf("%c", &type);
        fflush(stdin);
        if(type=='B')
        {
            strcpy(last->travelType,"Business");
            checker=0;
        }
        else if(type=='H')
        {
            strcpy(last->travelType,"Holiday");
            checker=0;
        }
        else if(type=='E')
        {
            strcpy(last->travelType,"Education");
            checker=0;
        }
        else
        {
            printf("\nInvalid input,try again!:");
            fflush(stdin);
        }

    }

    last->next=NULL; //same assignment as I did before to ensure linked list stops at the end.


}

void deleteTravel (struct tas *aa, int del)
{
    struct tas *temp, *temp_next, *deleted;
    temp=aa;

    

    if(del==1)
    {
        deleted=aa;
        aa=aa->next;
        free(deleted);
        printf("\nTravel with ID 1 has been deleted from your list!");
    }

    

    else
    {
        while(temp!=NULL && del!=temp->next->uniqueID)
        {
            temp=temp->next;
        }

        if(temp==NULL) //if we reach end of the list, then ID doesn't exist.
        {
            printf("\nID does not exist in the list.\n");
        }

        else
        {
            deleted=temp->next; //deleted node is set to deleted so we can delete it.

            temp_next=temp->next->next; //set the temp_next to the element after the deleted one.
            temp->next=temp_next; //point the element previous of delete to the after the deleted one.

            free(deleted); //remove the allocated node from memory

            printf("\nTravel with ID %d has been deleted from your list!", del);

        }

    }

}

void searchTravels(struct tas* aa)
{
    struct tas *searched;
    searched=aa;
    char type, type2;
    char country[20];
    int check=0; //check and check2 for checking if input is B/H/E and C/T or not.
    int check2=0;

    int foundornot=0;

    //"basic" input check and searching loop.
    //update: actually a big loop for whole searching part, including error checks and all. :)
    while(check==0)
    {
        printf("\nEnter your search option(C/T):");
        scanf("%c", &type);
        fflush(stdin);
        if(type=='C' || type=='c') //search by country here
        {

            check=1;
            printf("\nEnter country name:");
            scanf("%s", country);
            fflush(stdin);
            while(searched!=NULL)
            {
                if(strcmp(searched->countryName,country)==0)
                {
                    printf("\n%d;%s;%s;%s;%s", searched->uniqueID,searched->countryName,searched->departureDate,searched->returnDate,searched->travelType);
                    foundornot++;
                }
                searched=searched->next;
            }
            if(foundornot==0)
            {
                printf("Travel to %s is not available!", country);
            }
        }
        else if(type=='T' || type=='t') //search by type here.
        {
            while(check2==0)
            {
                printf("Enter search type (B/H/E):");
                scanf("%c", &type2);
                fflush(stdin);
                if(type2=='B' || type2=='b')
                {
                    check2=1;
                    while(searched!=NULL)
                    {
                        if(strcmp(searched->travelType,"Business")==0)
                        {
                            printf("\n%d;%s;%s;%s;%s", searched->uniqueID,searched->countryName,searched->departureDate,searched->returnDate,searched->travelType);
                        }
                        searched=searched->next;
                    }

                }
                else if(type2=='H' || type2=='h')
                {
                    check2=1;
                    while(searched!=NULL)
                    {
                        if(strcmp(searched->travelType,"Holiday")==0)
                        {
                            printf("\n%d;%s;%s;%s;%s", searched->uniqueID,searched->countryName,searched->departureDate,searched->returnDate,searched->travelType);
                        }
                        searched=searched->next;
                    }

                }
                else if(type2=='E' || type2=='e')
                {
                    check2=1;
                    while(searched!=NULL)
                    {
                        if(strcmp(searched->travelType,"Education")==0)
                        {
                            printf("\n%d;%s;%s;%s;%s", searched->uniqueID,searched->countryName,searched->departureDate,searched->returnDate,searched->travelType);
                        }
                        searched=searched->next;
                    }

                }
                else
                {
                    printf("\nYou enter wrong search type!"); //very advanced error check :)
                }

            }

            check=1;
        }
        else
        {
            printf("\nYou enter wrong search option!"); //another very advanced error check :)
        }
    }

}

void createSpecialList(struct tas* aa, char traveltype)
{
    struct tas *special;
    special=aa;

    FILE *text;

    //code below will create a special text file, which it's name depends on the travel type.

    if(traveltype=='B' || traveltype=='b')
    {
        text=fopen("Business.txt", "w");
        while(special!=NULL)
        {
            if(strcmp("Business",special->travelType)==0)
            {
                fprintf(text,"%d;%s;%s;%s;%s\n", special->uniqueID,special->countryName,special->departureDate,special->returnDate,special->travelType);
            }
            special=special->next;
        }
        printf("\nThe special list for Business type travels has been created and stored in the file: Business.txt!");


        fclose(text);
    }
    else if(traveltype=='H' || traveltype=='h')
    {
        text=fopen("Holiday.txt", "w");
        while(special!=NULL)
        {
            if(strcmp("Holiday",special->travelType)==0)
            {
                fprintf(text,"%d;%s;%s;%s;%s\n", special->uniqueID,special->countryName,special->departureDate,special->returnDate,special->travelType);
            }
            special=special->next;
        }
        printf("\nThe special list for Holiday type travels has been created and stored in the file: Holiday.txt!");


        fclose(text);
    }
    else if(traveltype=='E' || traveltype=='e')
    {
        text=fopen("Education.txt", "w");
        while(special!=NULL)
        {
            if(strcmp("Education",special->travelType)==0)
            {
                fprintf(text,"%d;%s;%s;%s;%s\n", special->uniqueID,special->countryName,special->departureDate,special->returnDate,special->travelType);
            }
            special=special->next;
        }
        printf("The special list for Education type travels has been created and stored in the file: Education.txt!");

        fclose(text);
    }
    else
    {
        printf("\nUnknown input!");
    }
}

void Overwrite(struct tas*aa)
{

    //at the end of all operations, travelArchive.txt should be updated, below code does just that.
    FILE *update;
    struct tas *write;
    write=aa;
    update=fopen("travelArchive.txt", "w");
    while(write!=NULL)
    {
        fprintf(update,"%d;%s;%s;%s;%s\n", write->uniqueID,write->countryName,write->departureDate,write->returnDate,write->travelType);
        write=write->next;
    }

    fclose(update);
}
