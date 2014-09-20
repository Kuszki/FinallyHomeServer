/*! \file
    \brief Plik zawierający specyfikację szablonów importowanych ze środowiska KuszkAPI.

Dołączany do KuszkAPI.cpp za pomocą odpowiedniej definicji umożliwia skompilowanie szablonów w odpowiedniej konfiguracji by uniknąć błędów linkera.
*/

template class MAP<int, STR>;
template class MAP<STR, int>;
template class MAP<STR, unsigned>;

template class ARA<char>;

template class THR;
template class SEC;

template unsigned CLI::Send<char>(const ARA<char>&, unsigned);

