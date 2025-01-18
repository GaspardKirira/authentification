#include <iostream>
#include "Profile.hpp"
#include "Contact.hpp"
#include "User.hpp"

int main()
{
    Softadastra::User s1("Gaspard", "Kirira", "kampala", "gaspardkirira@gmail.com", "+256783345232", "123");
    s1.display();

    return 0;
}