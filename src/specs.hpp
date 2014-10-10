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

template void KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TabBox>::Add(unsigned);
template void KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Label>::Add(unsigned);

template KuszkAPI::Forms::TabBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TabBox>::operator[] (unsigned);
template KuszkAPI::Forms::Label& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Label>::operator[] (unsigned);
