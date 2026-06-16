#include "core/page_table.hpp"

#include <utility>

namespace clawvm::core {

void PageTable::upsert(Page page) {
  pages_.insert_or_assign(page.id, std::move(page));
}

Page *PageTable::get(const std::string &id) {
  auto it = pages_.find(id);
  if (it == pages_.end()) {
    return nullptr;
  }
  return &it->second;
}

const Page *PageTable::get(const std::string &id) const {
  auto it = pages_.find(id);
  if (it == pages_.end()) {
    return nullptr;
  }
  return &it->second;
}

std::vector<Page *> PageTable::all_pages() {
  std::vector<Page *> result;
  result.reserve(pages_.size());

  for (auto &[_, page] : pages_) {
    result.push_back(&page);
  }

  return result;
}

std::vector<Page *> PageTable::dirty_pages() {
  std::vector<Page *> result;

  for (auto &[_, page] : pages_) {
    if (page.dirty) {
      result.push_back(&page);
    }
  }

  return result;
}

bool PageTable::contains(const std::string &id) const {
  return pages_.contains(id);
}

std::size_t PageTable::size() const { return pages_.size(); }

} // namespace clawvm::core
