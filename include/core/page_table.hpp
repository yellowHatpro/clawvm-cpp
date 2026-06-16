#pragma once

#include "core/page.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace clawvm::core {

class PageTable {
public:
  void upsert(Page page);

  Page *get(const std::string &id);
  const Page *get(const std::string &id) const;

  std::vector<Page *> all_pages();
  std::vector<Page *> dirty_pages();

  bool contains(const std::string &id) const;
  std::size_t size() const;

private:
  std::unordered_map<std::string, Page> pages_;
};
} // namespace clawvm::core
