template<typename T>
std::string to_string( const T & Value )
{
    // utiliser un flux de sortie pour cr�er la cha�ne
    std::ostringstream oss;
    // �crire la valeur dans le flux
    oss << Value;
    // renvoyer une string
    return oss.str();
}