#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct _retire_info retire_info;

//double current_balance(int start, double init_balance, retire_info saving) {
//  for (int i = start; i < start + saving.months; i++) {
//    printf("Age %3d month %2d you have $%.2lf\n", i / 12, i % 12, init_balance);
//    init_balance = init_balance * (1 + saving.rate_of_return / 12) + saving.contribution;
//  }
//}

void retirement(int startAge,         //in months
                double initial,       //initial savings in dollars
                retire_info working,  //info about working
                retire_info retired)  //info about being retired
{
  //double interm = current_balance(startAge, initial, working);
  //double final = current_balance(startAge + working.months, interm, retired);
  for (int i = startAge; i < startAge + working.months; i++) {
    printf("Age %3d month %2d you have $%.2lf\n", i / 12, i % 12, initial);
    initial = initial * (1 + working.rate_of_return / 12) + working.contribution;
  }

  for (int j = startAge + working.months; j < startAge + retired.months + working.months;
       j++) {
    printf("Age %3d month %2d you have $%.2lf\n", j / 12, j % 12, initial);
    initial = initial * (1 + retired.rate_of_return / 12) + retired.contribution;
  }
}

int main() {
  int start_age = 327;
  double init_balance = 21345;

  retire_info work;
  work.months = 489;
  work.contribution = 1000;
  work.rate_of_return = 0.045;

  retire_info retire;
  retire.months = 384;
  retire.contribution = -4000;
  retire.rate_of_return = 0.01;

  retirement(start_age, init_balance, work, retire);

  return 0;
}
