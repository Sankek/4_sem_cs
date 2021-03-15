#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

std::string read(std::istream &in){
  std::stringstream sstream;
  sstream << in.rdbuf();
  return sstream.str();
}

void save(std::ostream &out, const std::string &str){
  std::stringstream sstream;
  sstream << str;
  out << sstream.rdbuf();
}

int main(int argc, char** agrv){

  std::fstream file("../test.txt", std::ios::in);
  std::string content = read(file);
  file.close();

  enum{
    FIRST_SLASH,      // There was a single slash. If next symbol is not '/' or '*' -> marked 'false'
    SECOND_SLASH,     // There was the second consecutive slash meaning comment "//"
    OPEN_ASTERISK,    // There was "/*" construction so the next is commented until */ or EOF
    OPEN_QUOTE,       // There was an open quote meaning start of the string literal
    STATES_NUM
  };


  std::vector<bool> state(STATES_NUM, false);
  std::string::iterator del_it;

  for(auto it = content.begin(); it != content.end(); it++){
    switch(*it)
    {
      case '/': {
        if (state[OPEN_QUOTE] || state[SECOND_SLASH] || state[OPEN_ASTERISK])
        {
          break;
        }
        if (state[FIRST_SLASH])
        {
          state[FIRST_SLASH] = false;
          state[SECOND_SLASH] = true;
          del_it = it-1;
          break;
        }
        state[FIRST_SLASH] = true;
        break;
      }
      case '\n': {
        if (state[SECOND_SLASH])
        {
          if (*(it-1) == '\\')
          {
            break;
          }
          it = content.erase(del_it, it);
          state[FIRST_SLASH] = false;
          state[SECOND_SLASH] = false;
          break;
        }
        break;
      }
      case '"': {
        if (state[SECOND_SLASH] || state[OPEN_ASTERISK])
        {
          break;
        }
        if (state[OPEN_QUOTE])
        {
          if (*(it-1) == '\\')
          {
            if (*(it-2) == '\\')
            {
              state[OPEN_QUOTE] = false;
            }
            break;
          }
          state[OPEN_QUOTE] = false;
          break;
        }
        state[OPEN_QUOTE] = true;
        break;
      }
      case '*': {
        if (state[SECOND_SLASH])
        {
          break;
        }
        if (!state[OPEN_ASTERISK] && state[FIRST_SLASH])
        {
          state[OPEN_ASTERISK] = true;
          del_it = it-1;
          break;
        }
        if (state[OPEN_ASTERISK])
        {
          if ((it+1) != content.end() && *(it+1) == '/')
          {
            state[OPEN_ASTERISK] = false;
            it = content.erase(del_it, it+2);
            break;
          }
          break;
        }
        break;
      }
      default: {
        state[FIRST_SLASH] = false;
        break;
      }
    }
  }

  if (state[OPEN_ASTERISK]){
    content.erase(del_it, content.end());
  }

  file.open("../output.txt", std::ios::out);
  save(file, content);

  return EXIT_SUCCESS;
}