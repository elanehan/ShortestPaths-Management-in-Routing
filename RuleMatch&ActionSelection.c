#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    FILE *fp;
    int num_rules, num_ip, count, rules_list, change;

    // read the file
    fp = fopen("input.txt", "r");
    fscanf(fp, "%d", &num_rules);

    char rules[num_rules][3][12];
    for(count=0; count<num_rules; count++)
      fscanf(fp, "%s %s %s", &rules[count][0], &rules[count][1], &rules[count][2]);
    fscanf(fp, "%d", &num_ip);

    char ip[num_ip][3][12];
    for(count=0; count<num_ip; count++)
      fscanf(fp, "%s %s", &ip[count][0], &ip[count][1]);
    fclose(fp);

    //compare the rules and ip
    for(count=0; count<num_ip; count++)
    {
        change = 0;
        for(rules_list=0; rules_list<num_rules; rules_list++)
        {
            if( strcmp(ip[count][0], rules[rules_list][0]) == 0 && strcmp(ip[count][1], rules[rules_list][1]) == 0 )
            {
                strcpy(ip[count][2], rules[rules_list][2]);
                change = 1;
                break;
            }
            else if( strcmp(ip[count][0], rules[rules_list][0]) == 0 && strcmp(rules[rules_list][1], "*") == 0 )
            {
                strcpy(ip[count][2], rules[rules_list][2]);
                change = 1;
                break;
            }
            else if( strcmp(rules[rules_list][0], "*") == 0 && strcmp(ip[count][1], rules[rules_list][1]) == 0 )
            {
                strcpy(ip[count][2], rules[rules_list][2]);
                change = 1;
                break;
            }
            else if( strcmp(rules[rules_list][0], "*") == 0 && strcmp(rules[rules_list][1], "*") == 0 )
            {
                strcpy(ip[count][2], rules[rules_list][2]);
                change = 1;
                break;
            }
        }
        // to judge whether the ip has the matching rules; if not, place the drop inside
        if( change == 0 )
            strcpy(ip[count][2], "drop");
    }

    // print the result
    FILE *outfp;
    outfp=fopen("output.txt", "w+");
    fprintf(outfp, "%d\n", num_ip);
    /*for(count=0; count<num_rules; count++)
      fprintf(outfp, "%s %s %s\n", rules[count][0], rules[count][1], rules[count][2]);*/
    for(count=0; count<num_ip; count++)
      fprintf(outfp, "%s %s %s\n", ip[count][0], ip[count][1], ip[count][2]);
    fclose(outfp);

    return 0;
}

