#include "minishell.h"

int is_valid_exit_arg(const char *arg)
{
    int i = 0;
    int digits = 0;
    
    // Handle leading plus or minus
    if (arg[i] == '+' || arg[i] == '-')
        i++;
    
    // Check if the rest are all digits
    while (arg[i])
    {
        if (!ft_isdigit(arg[i]))
            return 0;
        digits++;
        i++;
    }
    
    // Ensure there's at least one digit
    return digits > 0;
}

long long ft_atoll(const char *str)
{
    long long result = 0;
    int sign = 1;
    int i = 0;

    if (str[i] == '+' || str[i] == '-')
    {
        sign = (str[i] == '-') ? -1 : 1;
        i++;
    }

    while (str[i] && ft_isdigit(str[i]))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result * sign;
}

char *trim_quotes_and_spaces(char *str)
{
    while (*str == ' ') str++;  // Trim leading spaces
    int len = ft_strlen(str);
    while (len > 0 && str[len-1] == ' ') len--;  // Trim trailing spaces
    
    if (len >= 2 && ((str[0] == '"' && str[len-1] == '"') || (str[0] == '\'' && str[len-1] == '\'')))
    {
        str[len-1] = '\0';
        return str + 1;
    }
    str[len] = '\0';
    return str;
}

int handle_exit(char **args)
{
    int arg_count = 0;
    while (args[arg_count])
        arg_count++;

    printf("exit\n");

    if (arg_count == 1)
    {
        exit(0);
    }
    else if (arg_count >= 2)
    {
        char *arg = args[1];
        int sign = 1;
        char *num_start = arg;

        // Handle the case where minus is outside the quotes
        if (arg[0] == '-' && arg[1] == '"')
        {
            sign = -1;
            num_start = arg + 1;
        }
        else if (arg[0] == '+' && arg[1] == '"')
        {
            num_start = arg + 1;
        }

        char *trimmed = trim_quotes_and_spaces(num_start);
        
        // Check if the trimmed argument is a valid number
        if (!is_valid_exit_arg(trimmed))
        {
            ft_putendl_fd("minishell: exit: numeric argument required", 2);
            exit(255);
        }
        
        long long exit_code = ft_atoll(trimmed) * sign;
        
        if (arg_count > 2)
        {
            ft_putendl_fd("minishell: exit: too many arguments", 2);
            ft_putchar_fd('\n', 2); 
            return 1;  // Don't exit, return 1 to set the exit code
        }
        
        exit((exit_code & 0xFF));
    }
    
    return 0;  // Should never reach here
}