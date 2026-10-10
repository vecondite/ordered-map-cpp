#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <cstddef>

template<typename kT, typename vT> class ordered_map{
    using Entry = std::pair<const kT, vT>;
    using DataList = std::list<Entry>;
    using EntryIterator = typename DataList::iterator;
    using CEntryIterator = typename DataList::const_iterator;
    using LocationMap = std::unordered_map<kT, EntryIterator>;
    using LocationIterator = typename LocationMap::iterator;
    DataList data;
    LocationMap locs;
    public:
        ordered_map() = default;
        ordered_map(const ordered_map& other) : data(other.data) { for(EntryIterator it = data.begin(); it != data.end(); ++it) locs.emplace(it->first, it); }
        ordered_map& operator=(const ordered_map& other) {
            if(this!=&other){
                ordered_map temp(other);
                data.swap(temp.data);
                locs.swap(temp.locs);
            }
            return *this;
        }
        ordered_map(ordered_map&&) = default;
        ordered_map& operator=(ordered_map&&) = default;

        vT& operator[](const kT& key){
            LocationIterator val = locs.find(key);
            if(val!=locs.end()){
                return val->second->second;
            }else{
                EntryIterator newEntry = data.emplace(data.end(), key, vT{});
                locs.emplace(key, newEntry);
                return newEntry->second;
            }
        }
        void erase(const kT& key){
            LocationIterator val = locs.find(key);
            if(val!=locs.end()){
                data.erase(val->second);
                locs.erase(val);
            }
        }
        EntryIterator begin() { return data.begin(); }
        EntryIterator end() { return data.end(); }
        CEntryIterator begin() const { return data.begin(); } 
        CEntryIterator end() const { return data.end(); }
        bool contains(const kT& key) const { return locs.find(key)!=locs.end(); }
        std::size_t size() const { return data.size(); }
        bool empty() const { return data.empty(); }
        void clear() { data.clear(); locs.clear(); }
};
