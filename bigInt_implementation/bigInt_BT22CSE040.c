/*conventions used
    ->the sign is 0 when the number is positive an it is 1 when the number is negative 
    ->the sign is required when the number is being passed as input for better representation and operations 
    ->if sign is not provided in the input then the program would not execute further
*/

/*the base of the numbers used for the calculations is 10^9 to optimise the storage used*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//the maximum size of a number will be 309, so the length of the string would be 311
#define SIZE 311
//taking thwe base for the calculations as 10^9
#define MOD_VAL 1000000000

typedef enum{FAILURE, SUCCESS} status_code;

int max(int a, int b){
    int retVal = b;
    if(a >= b){
        retVal = a;
    }

    return retVal;
}

typedef struct BIG_INT_TAG{
    int sign;
    int* arr;
    int length;
}bigInt;


//function to convert the given string into bigInt
//the number of digits in each index of arr would be 9 i.e. number stored in each index <= 99999999
status_code convertIntoNum(char* ch, bigInt* b){
    status_code retVal = SUCCESS;
    int strLength = strlen(ch);
    int count = 0;
    int numLength = ((strLength-1)/9) + ((strLength-1)%9 != 0);
    //printf("%d  ", numLength);
    int multiplier = 1;
    int localNum = 0, endFlag = 0;

    if(ch[0] == '+'){
        b -> sign = 0;
    }
    else if(ch[0] == '-'){
        b -> sign = 1;
    }
    else{
        retVal = FAILURE;
    }

    if(retVal == SUCCESS){
        b -> length = numLength;
        int index = 0;
        b -> arr = (int*)malloc(sizeof(int) * (b -> length));

        for(int i=strLength - 1; i>=1; i--){
            int num = ch[i] - '0';
            localNum = localNum + (multiplier * num);
            count++;
            multiplier*=10;
            if(i == 1){
                endFlag = 1;
            }
            if(count == 9 || endFlag == 1){
                b -> arr[index] = localNum;
                count = 0;
                localNum = 0;
                multiplier = 1;
                index++;
            }
        }
    }

    return retVal;
}


//function to print the number 
void printNum(bigInt* b){
    int length = b -> length;
    int j = length - 1;
    //parsing all the zeroes numbered indices
    while(j >= 0 && b -> arr[j] == 0){
        j--;
    }


    if(b -> sign == 0){
        printf("+");
    }
    else{
        printf("-");
    }
    int zero = 1;

    for(int i=j; i>=0; i--){
        int div = 100000000;
        while(div != 0 && i != j && b -> arr[i] / div == 0){
            printf("0");
            div = div / 10;
        }
        if(b -> arr[i] != 0){
            zero = 0;
            printf("%d", b -> arr[i]);
        }
        
        //printf("%d", b -> arr[i]);
    }

    if(zero == 1){
        printf("0");
    }

    printf("\n");
}


//declaration of the functions 
bigInt addition(bigInt* b1, bigInt* b2);
bigInt substraction(bigInt* b1, bigInt* b2);
bigInt multiply(bigInt* b1, bigInt* b2);





int main(){
    char* str1 = (char*)malloc(sizeof(char) * SIZE);
    char* str2 = (char*)malloc(sizeof(char) * SIZE);
    status_code s1, s2;
    bigInt b1, b2;
    bigInt ans;
    int operation;


    printf("Enter the first number \n");
    scanf("%s", str1);

    printf("Enter the second number \n");
    scanf("%s", str2);

    s1 = convertIntoNum(str1, &b1);
    s2 = convertIntoNum(str2, &b2);

    if(s1 == SUCCESS && s2 == SUCCESS){
        printf("Enter the number to perform the specific operation\n");
        printf("1. Addition     2. Subtraction     3. Multiplication \n");
        scanf("%d", &operation);

        if(operation == 1){
            ans = addition(&b1, &b2);
        }
        else if(operation == 2){
            ans = substraction(&b1, &b2);
        }
        else if(operation == 3){
            ans = multiply(&b1, &b2);
        }
        else{
            printf("Please specify the operation clearly \n");
        }

        if(operation == 1 || operation == 2 || operation == 3){
            printNum(&ans);
        }
    }
    else{
        printf("The number entered incorrectly, please enter the number again \n");
    }
    return 0;
}







bigInt addition(bigInt* b1, bigInt* b2){
    bigInt temp, ans;
    long long carry = 0, sum, i = 0;
    int length1 = b1 -> length, length2 = b2 -> length;

    //the addition is only required when the sign of both he numbers is same, else the result can be obtained by subtraction
    if(b1 -> sign == b2 -> sign){
        ans.sign = b1 -> sign;
        ans.length = max(b1 -> length, b2 -> length) + 1;
        ans.arr = (int*)malloc(sizeof(int) * (ans.length));

        while(i < length1 && i < length2){
            sum = carry + b1 -> arr[i] + b2 -> arr[i];
            ans.arr[i] = sum%MOD_VAL;
            carry = sum / MOD_VAL;
            i++;
        }
        while(i < length1){
            sum = carry + b1 -> arr[i];
            ans.arr[i] = sum % MOD_VAL;
            carry = sum / MOD_VAL;
            i++;
        }
        while(i < length2){
            sum = carry + b2 -> arr[i];
            ans.arr[i] = sum % MOD_VAL;
            carry = sum / MOD_VAL;
            i++;
        }

        ans.arr[i] = carry;
    }
    else if(b1 -> sign == 1){
        temp = *b1;
        temp.sign = 0;
        ans = substraction(b2, &temp);
    }
    else if(b2 -> sign == 1){
        temp = *b2;
        temp.sign = 0;
        ans = substraction(b1, &temp);
    }

    return ans;
}

bigInt substraction(bigInt* b1, bigInt* b2){
    bigInt temp, ans;
    long long diff, borrow = 0, carry = 0, i = 0, sum;
    int length1 = b1 -> length, length2 = b2 -> length;

    //the subtraction is only required when the sign of both the number is same, else the result can be obtained by addition
    if(b1 -> sign == b2 -> sign){
        ans.sign = b1 -> sign;
        ans.length = max(b1 -> length, b2 -> length) + 1;
        ans.arr = (int*)malloc(sizeof(int)*(ans.length));

        while(i < length1 && i < length2){
            diff = b1 -> arr[i] - borrow - b2 -> arr[i];
            if(diff < 0){
                diff += MOD_VAL;
                borrow = 1;
            }
            else{
                borrow = 0;
            }
            ans.arr[i] = diff;
            i++;
        }
        while(i < length1){
            diff = b1 -> arr[i] - borrow;
            if(diff < 0){
                borrow = 1;
                diff += MOD_VAL;
            }
            else{
                borrow = 0;
            }
            ans.arr[i] = diff;
            i++;
        }
        while(i < length2){
            borrow = 1;
            diff = -(borrow + b2 -> arr[i]);
            diff += MOD_VAL;
            ans.arr[i] = diff % MOD_VAL;
            i++;
        }

        if(borrow == 1){
            //answer is stored in 10^9-1's compliment form
            ans.arr[ans.length - 1] = MOD_VAL - 1;
            for(int i=0; i<ans.length; i++){
                ans.arr[i] = MOD_VAL - 1 - ans.arr[i];
            }
            ans.arr[0] += 1;
            carry = ans.arr[0] / MOD_VAL;
            ans.arr[0] = ans.arr[0] % MOD_VAL;

            i = 1;
            while(carry != 0){
                sum = carry + ans.arr[i];
                ans.arr[i] = sum % MOD_VAL;
                carry = sum / MOD_VAL;
                i++;
            }
            ans.sign = !(ans.sign);
        }
        else{
            ans.arr[ans.length-1] = 0;
        }
    }

    //call for addition if the sign of both the numbers is not same
    else{
        temp = *b2;
        temp.sign = !(temp.sign);
        ans = addition(b1, &temp);
    }

    return ans;
}

bigInt multiply(bigInt* b1, bigInt* b2){
    int length1 = b1 -> length;
    int length2 = b2 -> length;
    unsigned long long int i, j, sum, carry, num1, num2;
    bigInt m;

    //the maximum length of the number formed after multiplication will be equal to the length of both the numbers
    m.length = (length1 + length2);
    m.arr = (int*)malloc(sizeof(int) * (m.length));
    if(b1 -> sign == b2 -> sign){
        m.sign = 0;
    }
    else{
        m.sign = 1;
    }

    //initializing all the values of arr m to 0
    for(int i=0; i<m.length; i++){
        m.arr[i] = 0;
    }

    for(int i=0; i<length1; i++){
        carry = 0;
        for(int j=0; j<length2; j++){
            num1 = b1 -> arr[i];
            num2 = b2 -> arr[j];
            sum = carry + (m.arr[i+j]) + (num1 * num2);
            int entry = sum % MOD_VAL;
            m.arr[i+j] = entry;
            carry = sum / MOD_VAL;
        }
        if(carry != 0){
            m.arr[i+length2] = carry;
        }
    }

    return m;
}