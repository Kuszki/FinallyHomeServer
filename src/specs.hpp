/*! \file
    \brief Plik zawierający specyfikację szablonów importowanych ze środowiska KuszkAPI.

Dołączany do KuszkAPI.cpp za pomocą odpowiedniej definicji umożliwia skompilowanie szablonów w odpowiedniej konfiguracji by uniknąć błędów linkera.
*/

template class MAP<int, STR>;
template class MAP<STR, int>;
template class MAP<STR, unsigned>;

template class ARA<char>;

template class VCT<unsigned>;

template class THR;
template class SEC;

template unsigned CLI::Send<char>(const ARA<char>&, unsigned);

template KuszkAPI::Forms::TabBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TabBox>::Add(unsigned);
template KuszkAPI::Forms::Label& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Label>::Add(unsigned);
template KuszkAPI::Forms::TableBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TableBox>::Add(unsigned);
template KuszkAPI::Forms::Button& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Button>::Add(unsigned);
template KuszkAPI::Forms::CheckBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::CheckBox>::Add(unsigned);
template KuszkAPI::Forms::TrackBar& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TrackBar>::Add(unsigned);

template KuszkAPI::Forms::TabBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TabBox>::operator[] (unsigned);
template KuszkAPI::Forms::Label& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Label>::operator[] (unsigned);
template KuszkAPI::Forms::TableBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TableBox>::operator[] (unsigned);
template KuszkAPI::Forms::Button& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::Button>::operator[] (unsigned);
template KuszkAPI::Forms::CheckBox& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::CheckBox>::operator[] (unsigned);
template KuszkAPI::Forms::TrackBar& KuszkAPI::Forms::Controls::Group<KuszkAPI::Forms::TrackBar>::operator[] (unsigned);
