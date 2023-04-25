#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <map>
// The page table is implemented as an unordered map, mapping virtual
// addresses to physical addresses.
std::unordered_map<size_t, size_t> page_table;

// The memory is simulated as a vector of bytes.
std::vector<uint8_t> memory;

// This function translates a virtual address to a physical address, using
// the page table. If the virtual address is not present in the page table,
// it triggers a page fault and brings the page into memory.
 size_t translate_address(size_t virtual_address)
{
    // Look up the virtual address in the page table.
    auto it = page_table.find(virtual_address);
    if (it == page_table.end()) {
        // The virtual address is not present in the page table. Trigger a page
        // fault and bring the page into memory.

        // Calculate the page number and offset within the page.
        size_t page_size = 4096; // 4KB pages
        size_t page_number = virtual_address / page_size;
        size_t offset = virtual_address % page_size;

        // Allocate a new page of memory.
        size_t physical_address = memory.size();
        memory.resize(physical_address + page_size);

        // Add an entry to the page table for the new page.
        page_table[virtual_address] = physical_address;

        // Return the physical address of the new page.
        return physical_address + offset;
    }

    // The virtual address is present in the page table. Return the
    // corresponding physical address.
    return it->second;
}
 void Translate()
 {
     // Translate a few virtual addresses.
     std::cout << std::hex << translate_address(0x1000) << std::endl; // prints 0
     std::cout << std::hex << translate_address(0x2000) << std::endl; // prints 1000
     std::cout << std::hex << translate_address(0x3000) << std::endl; // prints 2000
     std::cout << std::hex << translate_address(0x4000) << std::endl; // prints 3000
 }



// Page table entry structure
struct PageTableEntry {
    int pageNumber;
    bool valid;
    int frameNumber;
};

// Page table class
class PageTable {
private:
    std::vector<PageTableEntry> entries;
    int numFrames;

public:
    PageTable(int numFrames) : numFrames(numFrames) {
        // Initialize page table entries with invalid values
        for (int i = 0; i < numFrames; i++) {
            PageTableEntry entry = { -1, false, -1 };
            entries.push_back(entry);
        }
    }

    // Find a page in the page table
    PageTableEntry* findPage(int pageNumber) {
        for (int i = 0; i < numFrames; i++) {
            if (entries[i].pageNumber == pageNumber && entries[i].valid) {
                return &entries[i];
            }
        }
        return nullptr;
    }

    // Allocate a new page in the page table
    bool allocatePage(int pageNumber, int frameNumber) {
        PageTableEntry* entry = findPage(pageNumber);
        if (entry) {
            // Page is already in the page table
            return false;
        }

        // Find an invalid entry in the page table
        for (int i = 0; i < numFrames; i++) {
            if (!entries[i].valid) {
                entries[i] = { pageNumber, true, frameNumber };
                return true;
            }
        }
        return false;
    }

    // Deallocate a page from the page table
    bool deallocatePage(int pageNumber) {
        PageTableEntry* entry = findPage(pageNumber);
        if (entry) {
            entry->valid = false;
            return true;
        }
        return false;
    }

    // Print the contents of the page table
    void printPageTable() {
        std::cout << "Page Number\tValid\tFrame Number" << std::endl;
        for (int i = 0; i < numFrames; i++) {
            std::cout << entries[i].pageNumber << "\t\t" << entries[i].valid
                << "\t\t" << entries[i].frameNumber << std::endl;
        }
    }
};
void PageTableFunction()
{
    // Create a page table with 3 frames
    PageTable pt(3);

    // Allocate pages in the page table
    pt.allocatePage(1, 0);
    pt.allocatePage(2, 1);
    pt.allocatePage(3, 2);

    // Print the page table
    pt.printPageTable();

    // Deallocate page 2 from the page table
    pt.deallocatePage(2);

    // Print the page table again
    pt.printPageTable();
}


// The page table is implemented as an unordered map, mapping virtual
// addresses to physical addresses.
std::unordered_map<size_t, size_t> page_table_tlb;

// The memory is simulated as a vector of bytes.
std::vector<uint8_t> memory_tlb;

// The TLB is implemented as an unordered map, mapping virtual addresses
// to physical addresses.
std::unordered_map<size_t, size_t> tlb;

// This function translates a virtual address to a physical address, using
// the TLB and the page table. If the virtual address is not present in the
// TLB or the page table, it triggers a page fault and brings the page into
// memory.
size_t translate_address_tlb(size_t virtual_address_tlb)
{
    // Look up the virtual address in the TLB.
    auto it = tlb.find(virtual_address_tlb);
    if (it != tlb.end()) {
        // The virtual address is present in the TLB. Return the corresponding
        // physical address.
        return it->second;
    }

    // The virtual address is not present in the TLB. Look it up in the page
    // table.
    it = page_table_tlb.find(virtual_address_tlb);
    if (it == page_table_tlb.end()) {
        // The virtual address is not present in the page table. Trigger a page
        // fault and bring the page into memory.

        // Calculate the page number and offset within the page.
        size_t page_size = 4096; // 4KB pages
        size_t page_number = virtual_address_tlb / page_size;
        size_t offset = virtual_address_tlb % page_size;

        // Allocate a new page of memory.
        size_t physical_address = memory_tlb.size();
        memory_tlb.resize(physical_address + page_size);

        // Add an entry to the page table for the new page.
        page_table_tlb[virtual_address_tlb] = physical_address;

        // Return the physical address of the new page.
        return physical_address + offset;
    }

    // The virtual address is present in the page table. Add an entry to the
    // TLB for the translation and return the corresponding physical address.
    tlb[virtual_address_tlb] = it->second;
    return it->second;
}

void Tlb()
{
    // Translate a few virtual addresses.
    std::cout << std::hex << translate_address_tlb(0x1000) << std::endl; // prints 0
    std::cout << std::hex << translate_address_tlb(0x2000) << std::endl; // prints 1000
    std::cout << std::hex << translate_address_tlb(0x3000) << std::endl; // prints 2000
    std::cout << std::hex << translate_address_tlb(0x4000) << std::endl; // prints 3000

}


const int PHYSICAL_MEMORY_SIZE = 1024;  // Physical memory size in bytes
void PhysicalMemory()
{
    // Declare an array to represent the physical memory
    int physical_memory[PHYSICAL_MEMORY_SIZE];

    // Initialize the physical memory with some sample data
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        physical_memory[i] = i;
    }

    // Print out the contents of the physical memory
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        std::cout << "Physical memory location " << i << ": " << physical_memory[i] << std::endl;
    }
}


void placing()
{
    // Declare a map to represent the virtual memory
    std::map<int, int> virtual_memory;

    // Initialize the virtual memory with some sample data
    for (int i = 0; i < 1024; i++) {
        virtual_memory[i] = i;
    }

    // Retrieve a value from the virtual memory
    int virtual_address = 42;
    int value = virtual_memory[virtual_address];
    std::cout << "Value at virtual memory address " << virtual_address << ": " << value << std::endl;

}



const int PAGE_SIZE = 4096; // page size in bytes
const int NUM_PAGES = 100; // number of pages in virtual memory
void placing_page()
{
    // create a map to represent the page table
    std::map<int, int> page_table;

    // initialize the page table with invalid entries
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = i;
    }

    // place a page in main memory
    int virtual_page_number = 42;
    int physical_page_number = 10;
    page_table[virtual_page_number] = physical_page_number;

    // print out the page table
    for (int i = 0; i < NUM_PAGES; i++) {
        std::cout << "Virtual page " << i << " is mapped to physical page " << page_table[i] << std::endl;
    }

}

void replacing()
{
    // create a map to represent the page table
    std::map<int, int> page_table;

    // initialize the page table with invalid entries
    for (int i = 0; i < NUM_PAGES; i++) {
        page_table[i] = -1;
    }

    // place some pages in main memory
    page_table[0] = 0;
    page_table[1] = 1;
    page_table[2] = 2;
    page_table[3] = 3;

    // create a list to represent the main memory
    std::list<int> main_memory;

    // initialize the main memory with the pages from the page table
    for (int i = 0; i < NUM_PAGES; i++) {
        if (page_table[i] != -1) {
            main_memory.push_back(page_table[i]);
        }
    }
    // print out the main memory
    std::cout << "First Main memory: ";
    for (int page : main_memory) {
        std::cout << page << " ";
    }
    std::cout << std::endl;
    // replace a page in main memory
    int virtual_page_number = 2;
    int physical_page_number = 10;
    page_table[virtual_page_number] = physical_page_number;

    // update the main memory with the new page
    main_memory.remove(virtual_page_number);
    main_memory.push_back(physical_page_number);

    // print out the main memory
    std::cout << "After replace Main memory: ";
    for (int page : main_memory) {
        std::cout << page << " ";
    }
    std::cout << std::endl;

}

void back()
{
    std::cout << "\n-----------------------------------------\nMenu\n";
    std::cout << "Press 1 for Page Table\n";
    std::cout << "Press 2 for Translate from virtual adress to physical adress\n";
    std::cout << "Press 3 for Translate with Tlb\n";
    std::cout << "Press 4 for Physical Memory\n";
    std::cout << "Press 5 for Placing in memory\n";
    std::cout << "Press 6 for Placing a page in main memory\n";
    std::cout << "Press 7 for Replacing page\n";
    int x, y;
    std::cin >> x;
    if (x == 1)
    {
        PageTableFunction();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 2)
    {
        Translate();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 3)
    {
        Tlb();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 4)
    {
        PhysicalMemory();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 5)
    {
        placing();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 6)
    {
        placing_page();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 7)
    {
        replacing();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }

}
void menu()
{
    std::cout << "Menu\n";
    std::cout << "Press 1 for Page Table\n";
    std::cout << "Press 2 for Translate from virtual adress to physical adress\n";
    std::cout << "Press 3 for Translate with Tlb\n";
    std::cout << "Press 4 for Physical Memory\n";
    std::cout << "Press 5 for Placing in memory\n";
    std::cout << "Press 6 for Placing a page in main memory\n";
    std::cout << "Press 7 for Replacing page\n";
    int x,y;
    std::cin >> x;
    if (x == 1)
    {
        PageTableFunction();
        std::cout << "\n------------------------------\nFor back press 0\n";
            std::cin >> y;
            if (y == 0)
                back();
    }
    if (x == 2)
    {
        Translate();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 3)
    {
        Tlb();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 4)
    {
        PhysicalMemory();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 5)
    {
        placing();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 6)
    {
        placing_page();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
    if (x == 7)
    {
        replacing();
        std::cout << "\n------------------------------\nFor back press 0\n";
        std::cin >> y;
        if (y == 0)
            back();
    }
   
}

int main() 
{
    menu();
    return 0;
}


