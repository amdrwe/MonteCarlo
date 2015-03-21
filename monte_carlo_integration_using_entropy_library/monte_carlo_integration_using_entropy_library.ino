#include <Entropy.h>

#include <StackList.h>//need a stack to evaluate our expression; easier than writing an infix parser or doing it myself. bog-standard stack implemented using linked list

unsigned long g_last_random; // last random value
//i dont know why i'm doing this on an arduino, but the due is kinda neat for math stuff like this
//don't bother trying this on anything but a 32-bit board (e.g. the Due); the loss of precision from floats would be egregious in addition to the considerably slower clock speed et al.
void setup() {
  Serial.begin(9600);
  
  //init_real_random();
  monte_carlo_integrate("2x*", 0, 10, 200000, 1);
}
void loop(){  }
//expression must be postfix, at current time only supports one digit numbers and one variable [a-zA-z]
//lower_bound is self explanatory, as is upper_bound
//counter is a number specifying how many times you want to put a point in the box, basically. It's a long in case you want to do more than max_int calculations (recommended)
//I found good results with 100,000 to 200,000 iterations; be warned that beyond that it takes a _very_ long time to calculate the result (or at least more time than I was willing to wait)
//interval is the delta used for finding the approximate max value of the function over the interval; 0.01 is probably a good place to start
double monte_carlo_integrate(String expression, int lower_bound, int upper_bound, long counter, double interval){
  Serial.print("Doing Monte Carlo integration of ");
  Serial.println(expression);
  Serial.print("Lower bound: ");
  Serial.println(lower_bound);
  Serial.print("Upper bound: ");
  Serial.println(upper_bound);
  Serial.print("Check count: ");
  Serial.println(counter);
  int hits = 0;
  Serial.print("Finding approx max: ");
  double approx_max = find_max(expression, interval, lower_bound, upper_bound); 
  Serial.println(approx_max);
  for (long i = 0; i < counter; i++) {
    double rand_x;
    do{
      //rand_x = random_number() * upper_bound;
      double to_abs = upper_bound - lower_bound;
      rand_x = Entropy.rnorm((upper_bound - lower_bound / 2.0), (abs(to_abs)/2.0));
    } 
    while (rand_x < lower_bound);
    double rand_y = random_number() * approx_max;

      double result = postfix_eval(expression, rand_x);
    if (rand_y <= result){
      hits++;
    }
    Serial.print(".");
  }
  Serial.print("hits: "); 
  Serial.println(hits);
  Serial.print("total number of attempts: "); 
  Serial.println(counter);
  double proportion = (double(hits)/double(counter));
  Serial.print("Proportion of hits: "); 
  Serial.println(proportion,5);
  double box_area = ((upper_bound - lower_bound) * approx_max);
  Serial.print("Area of rectangle: "); 
  Serial.println(box_area, 5);
  double estimated_result = proportion * box_area;
  Serial.print("Estimated result: "); 
  Serial.println(estimated_result,5);
  return estimated_result;
}

double postfix_eval(String expression, double var){ //use a stack to evaluate a postfix expression. var is what it will put on the stack if it finds a character other than an operator or a digit.
  double y;
  double y1;
  double y2;
  StackList<double> stack;
  char ch;
  for (int i = 0; i < expression.length(); i++){
    ch = expression.charAt(i);
    if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z')
    {
      stack.push(var);
    } 
    else
      if(ch >= '0' && ch <= '9'){
        stack.push(ch - 48);
      } 
      else 
        if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
      {  
        y2 = stack.pop();
        y1 = stack.pop();
        if(ch == '+')
        {
          y = y1 + y2;
          stack.push(y);
        }
        if(ch == '-')
        {
          y = y1 - y2;
          stack.push(y);
        }
        if(ch == '^')
        {
          y = pow(y1, y2); //^ is the bitwise xor op for some asinine reason, so pow is used. returns a double so no loss of precision.
          stack.push(y);
        }
        if(ch == '*')
        {
          y = y1 * y2;
          stack.push(y);
        }
        if(ch=='/')
        {
          y = y1 / y2;
          stack.push(y);
        } 
      }
  }
  double r = stack.pop();
  return r;
}

double find_max(String expression, double interval, int lower_bound, int upper_bound){ //find the max value so we have a vertical size for our box
  double return_this = 0;//initialize our return to 0
  double current_val = lower_bound;
  double temp;
  while (current_val <= upper_bound){
    temp = postfix_eval(expression, current_val);
    if(temp >= return_this){
      return_this = temp;
    }
    current_val+=interval;
  }
  return return_this;//duh
}


void init_real_random()//initialize our prng
{
  g_last_random = millis();//reasonably random seed; not perfect but better than the godawful builtin random algo
}

double random_number() //random() (the default arduino prng) is a pos, this provides more "random" results and also returns a double between 0 and 1, which is more useful
{
  unsigned long temp;
  temp = 663608941*g_last_random; //multiply a big number by our last random number
  g_last_random = temp; //new random
  return(((double)temp)/((double)4294967295)); //make it a small number; this is essentially a prototypical prng but other implementations (i.e. seed based on a0 voltage) didn't pan out
}



