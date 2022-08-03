

class BigInt
{
private:
    string num; // string is stored in reverse order

public:
    //Constructors
    BigInt(string s) // parameterized Constructor
    {
        num = "";
        if (s[0] != '-')
        {
            for (int i = s.size() - 1; i >= 0; i--)
            {
                if (isdigit(s[i]) == false)
                {
                    throw("ERROR");
                }
                num += s[i];
            }
        }
        else
        {
            for (int i = s.size() - 1; i > 0; i--)
            {
                if (isdigit(s[i]) == false)
                {
                    throw("ERROR");
                }
                num += s[i];
            }
            num += "-";
        }
    }
    BigInt(const BigInt &val) // Copy Constructor
    {
        num = val.num;
    }
    BigInt() // Default constructor
    {
        num = "0";
    }
    BigInt(long long int number) // another parameterized constructor
    {
        num = "";
        bool check_sign = true;
        if (number < 0)
        {
            number = (-1) * number;
            check_sign = false;
        }
        while (number > 0)
        {
            num += ((number % 10) + '0');
            number /= 10;
        }
        if (check_sign == false)
        {
            num += "-";
        }
    }

    //overloaded cout(<<) and cin(>>)
    friend ostream &operator<<(ostream &out, const BigInt &c);
    friend istream &operator>>(istream &out, BigInt &c);
    friend BigInt operator+(long long int val, BigInt c);
    friend BigInt operator-(long long int val, BigInt c);
    friend BigInt operator+=(long long int val, BigInt c);
    friend BigInt operator-=(long long int val, BigInt c);

    //Addition
    BigInt operator+(BigInt const &obj)
    {
        BigInt temp;
        temp.num = "";

        if (obj.num[obj.num.size() - 1] == '-' && num[num.size() - 1] == '-')
        {
            BigInt temp1, temp2;

            temp1.num = num.substr(0, num.size() - 1);
            temp2.num = obj.num.substr(0, obj.num.size() - 1);

            temp = temp1 + temp2; // recursive call to operator+() function
            temp.num.push_back('-');
        }
        else if (num[num.size() - 1] == '-') // obj is +ve and calling object has a -ve number
        {
            BigInt temp1, temp2;

            temp1.num = num.substr(0, num.size() - 1);
            temp2.num = obj.num.substr(0, obj.num.size());

            temp = temp2 - temp1;
        }
        else if (obj.num[obj.num.size() - 1] == '-') // passed object has a -ve string and calling object has a +ve sign
        {
            BigInt temp1, temp2;

            temp1.num = num.substr(0, num.size());
            temp2.num = obj.num.substr(0, obj.num.size() - 1);

            temp = temp1 - temp2;
        }
        else
        {
            int size1 = num.size();
            int size2 = obj.num.size();

            int size = min(size1, size2);
            int carry = 0;
            int sum, digit;

            for (int i = 0; i < size; i++)
            {
                int num1 = num[i] - '0';
                int num2 = obj.num[i] - '0';

                // Chars under the hood are integers, and so you can go through them with arithmetic operator.
                // char c = "7"; ---> internally stored as integers(as ASCII value)
                // c - '0' subtracts the ASCII value of 0 from character c
                // c - '0' is an type of integer because it is internally difference of two integers(ASCII value)
                // So the output is an integer

                sum = num1 + num2 + carry;
                carry = sum / 10;
                digit = sum % 10;

                char d = digit + '0'; // convert digit(int) to char

                temp.num.push_back(d);
            }

            // For leftover digits in any of the string

            if (size == size1) // object passed has parameter has bigger string size
            {
                for (int i = size1; i < size2; i++)
                {
                    int num3 = obj.num[i] - '0';
                    sum = num3 + carry;
                    carry = sum / 10;
                    digit = sum % 10;

                    char d = digit + '0';
                    temp.num.push_back(d);
                }
            }
            else if (size == size2) // object calling the operator function has bigger string size
            {
                for (int i = size2; i < size1; i++)
                {
                    int num3 = num[i] - '0';
                    sum = num3 + carry;
                    carry = sum / 10;
                    digit = sum % 10;

                    char d = digit + '0';
                    temp.num.push_back(d);
                }
            }

            if (carry == 1)
            {
                char d = carry + '0';
                temp.num.push_back(d);
            }
        }
        return temp;
    }
    void operator+=(BigInt const &obj)
    {
        *this = *this + obj;
    }

    // Subtraction
    BigInt operator-(BigInt op2)
    {

        int len1 = num.length();
        int len2 = (op2.num).length();
        if (num[len1 - 1] == '-' && op2.num[len2 - 1] != '-')
        {
            BigInt res, temp;
            temp.num = this->num;
            num = num.substr(0, len1 - 1);
            res = this->operator+(op2);
            res.num += '-';
            this->num = temp.num;
            return res;
        }
        else if (num[len1 - 1] != '-' && op2.num[len2 - 1] == '-')
        {
            BigInt res, temp;
            temp.num = this->num;
            op2.num = op2.num.substr(0, len2 - 1);
            res = this->operator+(op2);
            this->num = temp.num;
            return res;
        }
        else if (num[len1 - 1] == '-' && op2.num[len2 - 1] == '-')
        {
            BigInt res, temp;
            temp.num = this->num;
            num = num.substr(0, len1 - 1);
            op2.num = op2.num.substr(0, len2 - 1);
            len1--;
            len2--;
            if (len2 > len1)
            {
                res = op2.ActSub(*this);
                this->num = temp.num;
                return res;
            }
            else if (len1 > len2)
            {
                res = this->ActSub(op2);
                res.num += "-";
                this->num = temp.num;
                return res;
            }
            else
            {
                int c = 0;
                for (int k = len1 - 1; k >= 0; k--)
                {
                    if (num[k] > op2.num[k])
                    {
                        res = this->ActSub(op2);
                        res.num += "-";
                        this->num = temp.num;
                        return res;
                    }
                    else if (num[k] < op2.num[k])
                    {
                        res = op2.ActSub(*this);
                        this->num = temp.num;
                        return res;
                    }
                    else
                    {
                        c++;
                    }
                }
                if (c == len1)
                {
                    res.num = "0";
                    this->num = temp.num;
                    return res;
                }
            }
        }
        else
        {
            BigInt res, temp;
            temp.num = this->num;
            if (len1 > len2)
            {
                res = this->ActSub(op2);
                this->num = temp.num;
                return res;
            }
            else if (len2 > len1)
            {
                res = op2.ActSub(*this);
                res.num += "-";
                this->num = temp.num;
                return res;
            }
            else
            {
                int c = 0;
                for (int k = len1 - 1; k >= 0; k--)
                {
                    if (num[k] > op2.num[k])
                    {
                        res = this->ActSub(op2);
                        this->num = temp.num;
                        return res;
                    }
                    else if (num[k] < op2.num[k])
                    {
                        res = op2.ActSub(*this);
                        res.num += "-";
                        this->num = temp.num;
                        return res;
                    }
                    else
                    {
                        c++;
                    }
                }
                if (c == len1)
                {
                    res.num = "0";
                    this->num = temp.num;
                    return res;
                }
            }
        }

        BigInt res;
        res.num = "0";
        return res;
    }
    void operator-=(BigInt const &obj)
    {
        *this = this->operator-(obj);
    }
    BigInt ActSub(BigInt operand)
    {
        BigInt temp;
        temp.num = this->num;
        int len = num.length();
        int len2 = operand.num.length();
        BigInt res;
        res.num = "";
        for (int i = 0; i < len; i++)
        {
            int a = num[i] - '0';
            int b;
            if (i <= len2 - 1)
                b = operand.num[i] - '0';
            else
                b = 0;
            if (a >= b)
                res.num += (char)(a - b + 48);
            else
            {
                int x = i + 1;
                while (x <= len - 1)
                {
                    if (num[x] != '0')
                    {
                        int y = num[x] - '0';
                        y--;
                        num[x] = (char)(y + 48);
                        a = a + 10;
                        res.num += (char)(a - b + 48);
                        for (int j = i + 1; j < x; j++)
                        {
                            num[j] = '9';
                        }
                        break;
                    }
                    else
                    {
                        x++;
                    }
                }
            }
        }

        int count = 0;
        int l = res.num.length();
        for (int j = l - 1; j >= 0; j--)
        {
            if (res.num[j] == '0')
                count++;
            else
                break;
        }
        res.num = res.num.substr(0, l - count);
        if (res.num == "")
            res.num = "0";
        this->num = temp.num;
        return res;
    }

    // Assignment operator
    BigInt operator=(string s)
    {
        num = "";
        if (s[0] != '-')
        {
            for (int i = s.size() - 1; i >= 0; i--)
            {
                if (isdigit(s[i]) == false)
                {
                    throw("ERROR");
                }
                num += s[i];
            }
        }
        else
        {
            for (int i = s.size() - 1; i > 0; i--)
            {
                if (isdigit(s[i]) == false)
                {
                    throw("ERROR");
                }
                num += s[i];
            }
            num += "-";
        }
    }
};

ostream &operator<<(ostream &out, const BigInt &c)
{
    for (int i = c.num.size() - 1; i >= 0; i--)
    {
        out << c.num[i];
    }
    return out;
}

istream &operator>>(istream &in, BigInt &c)
{
    string s;
    in >> s;
    c.num = "";
    if (s[0] != '-')
    {
        for (int i = s.size() - 1; i >= 0; i--)
        {
            if (isdigit(s[i]) == false)
            {
                throw("ERROR");
            }
            c.num += s[i];
        }
    }
    else
    {
        for (int i = s.size() - 1; i > 0; i--)
        {
            if (isdigit(s[i]) == false)
            {
                throw("ERROR");
            }
            c.num += s[i];
        }
        c.num += "-";
    }
    return in;
}

BigInt operator+(long long int val, BigInt c)
{
    BigInt temp(val);
    temp = temp + c;
    return temp;
}

BigInt operator-(long long int val, BigInt c)
{
    BigInt temp(val);
    temp = temp - c;
    return temp;
}

BigInt operator+=(long long int val, BigInt c)
{
    BigInt temp(val);
    temp += c;
    return temp;
}

BigInt operator-=(long long int val, BigInt c)
{
    BigInt temp(val);
    temp -= c;
    return temp;
}

