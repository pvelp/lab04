#include <header.hpp>

namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
  fs::path path_to_ftp;
  if (argc < 2) {
    path_to_ftp = fs::current_path();
  } else {
    path_to_ftp = argv[1];
  }

  //регулярное выражение, ограничивает название файла
  const std::regex balance_file{"balance_[0-9]{8}_[0-9]{8}\\.txt"};
  std::map<std::string, Broker> brokers; //словарь с брокерами

  //рекурсивно проходит по вложенным директориям
  for (const auto& path : fs::recursive_directory_iterator(path_to_ftp)) {
    //имя файла
    auto filename = path.path().filename();
    //имя брокера
    auto broker_name = path.path().parent_path().filename().string();
    //если текущий путь - файл и название вписывается в диапазон -> выводим
    //название, вырезаем и сохраняем дату и номер акка
    if (fs::is_regular_file(path) &&
        std::regex_match(filename.string(), balance_file)) {
      std::cout << filename.string() << std::endl;
      std::string date = filename.string();
      date.erase(0, 17);
      date.erase(8, 4);

      std::string num_account = filename.string();
      num_account.erase(0, 8);
      num_account.erase(8, 13);
      if (!brokers.count(broker_name)) { //если в словаре по данному ключу пусто
        Broker broker(broker_name, date, num_account);//создаем брокера
        brokers.insert({broker_name, broker}); //кладем его в словарь
      } else {
        brokers.at(broker_name).set_date(date); //иначе увеличиваем кол-во счетов
        ++brokers.at(broker_name);
      }
    }
  }
  for (auto &broker : brokers) {
    broker.second.print(); //для value словаря типа Broker используем метод
  }
  return 0;
}