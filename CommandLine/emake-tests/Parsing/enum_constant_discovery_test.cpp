#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <filesystem>
#include <vector>
#include <string>

// Declare extern for main_context which is initialized by BuiltinTypesEnvironment
extern clang_adapter::ClangContext* main_context;

namespace fs = std::filesystem;

class EnumConstantDiscoveryTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!main_context) {
            FAIL() << "main_context was not initialized";
        }
        
        lang_ = std::make_unique<lang_CPP>();
        
        const char* config = 
            "%e-yaml\n"
            "---\n"
            "target-windowing: None\n"
            "target-audio: None\n"
            "target-compiler: gcc\n"
            "target-graphics: None\n"
            "target-widget: None\n"
            "target-collision: None\n"
            "target-networking: None\n";
        lang_->definitionsModified(nullptr, config);
        
        // Load shared locals to populate the set that codegen will use
        int result = lang_->load_shared_locals();
        ASSERT_EQ(result, 0) << "load_shared_locals() should succeed";
    }
    
    void TearDown() override {
        lang_.reset();
    }
    
    std::unique_ptr<lang_CPP> lang_;
};

// Test that color constants from GScolors.h are discovered
TEST_F(EnumConstantDiscoveryTest, DiscoverColorConstants) {
    // Test c_blue - should be found in enigma_user namespace
    jdi::definition* c_blue = lang_->look_up("c_blue");
    ASSERT_NE(c_blue, nullptr) << "c_blue should be found by look_up()";
    ASSERT_NE(c_blue->parent, nullptr) << "c_blue should have a parent scope";
    ASSERT_EQ(c_blue->parent->name, "enigma_user") << "c_blue should be in enigma_user namespace";
    ASSERT_TRUE(c_blue->flags & jdi::DEF_TYPED) << "c_blue should have DEF_TYPED flag";
    ASSERT_FALSE(c_blue->flags & jdi::DEF_FUNCTION) << "c_blue should not be a function";
    
    // Test c_white - should be found in enigma_user namespace
    jdi::definition* c_white = lang_->look_up("c_white");
    ASSERT_NE(c_white, nullptr) << "c_white should be found by look_up()";
    ASSERT_NE(c_white->parent, nullptr) << "c_white should have a parent scope";
    ASSERT_EQ(c_white->parent->name, "enigma_user") << "c_white should be in enigma_user namespace";
    ASSERT_TRUE(c_white->flags & jdi::DEF_TYPED) << "c_white should have DEF_TYPED flag";
    ASSERT_FALSE(c_white->flags & jdi::DEF_FUNCTION) << "c_white should not be a function";
    
    // Test c_black - another color constant
    jdi::definition* c_black = lang_->look_up("c_black");
    ASSERT_NE(c_black, nullptr) << "c_black should be found by look_up()";
    ASSERT_NE(c_black->parent, nullptr) << "c_black should have a parent scope";
    ASSERT_EQ(c_black->parent->name, "enigma_user") << "c_black should be in enigma_user namespace";
}

// Test that self constant from object.h is discovered
TEST_F(EnumConstantDiscoveryTest, DiscoverSelfConstant) {
    // Test self - should be found in enigma_user namespace
    jdi::definition* self = lang_->look_up("self");
    ASSERT_NE(self, nullptr) << "self should be found by look_up()";
    ASSERT_NE(self->parent, nullptr) << "self should have a parent scope";
    ASSERT_EQ(self->parent->name, "enigma_user") << "self should be in enigma_user namespace";
    ASSERT_TRUE(self->flags & jdi::DEF_TYPED) << "self should have DEF_TYPED flag";
    ASSERT_FALSE(self->flags & jdi::DEF_FUNCTION) << "self should not be a function";
    
    // Test other related constants
    jdi::definition* other = lang_->look_up("other");
    ASSERT_NE(other, nullptr) << "other should be found by look_up()";
    ASSERT_EQ(other->parent->name, "enigma_user") << "other should be in enigma_user namespace";
    
    jdi::definition* all = lang_->look_up("all");
    ASSERT_NE(all, nullptr) << "all should be found by look_up()";
    ASSERT_EQ(all->parent->name, "enigma_user") << "all should be in enigma_user namespace";
}

// Test that enum constants are stored as direct namespace members (not inside enum type)
TEST_F(EnumConstantDiscoveryTest, EnumConstantsAreDirectNamespaceMembers) {
    // This test verifies that enum constants can be found via find_local on the namespace
    // If they're stored inside an enum type definition, find_local won't find them
    
    // We need access to namespace_enigma_user to test this
    // For now, we'll test via look_up which should work if they're direct members
    // The actual implementation will verify the storage structure
    
    jdi::definition* c_blue = lang_->look_up("c_blue");
    ASSERT_NE(c_blue, nullptr) << "c_blue must be findable";
    
    // Verify it's a direct member by checking parent
    ASSERT_EQ(c_blue->parent->name, "enigma_user") << "c_blue should be direct member of enigma_user";
}

// Test that multiple enum constants from same anonymous enum are all found
TEST_F(EnumConstantDiscoveryTest, MultipleConstantsFromSameEnum) {
    // All color constants should be found
    std::vector<std::string> color_constants = {
        "c_aqua", "c_black", "c_blue", "c_dkgray", "c_fuchsia",
        "c_gray", "c_green", "c_lime", "c_ltgray", "c_maroon",
        "c_navy", "c_olive", "c_orange", "c_purple", "c_red",
        "c_silver", "c_teal", "c_white", "c_yellow"
    };
    
    for (const auto& name : color_constants) {
        jdi::definition* def = lang_->look_up(name);
        ASSERT_NE(def, nullptr) << name << " should be found by look_up()";
        ASSERT_NE(def->parent, nullptr) << name << " should have a parent scope";
        ASSERT_EQ(def->parent->name, "enigma_user") 
            << name << " should be in enigma_user namespace";
        ASSERT_FALSE(def->flags & jdi::DEF_FUNCTION) 
            << name << " should not be a function";
    }
}
