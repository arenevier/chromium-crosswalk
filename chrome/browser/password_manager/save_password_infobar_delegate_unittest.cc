// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/password_manager/save_password_infobar_delegate.h"
#include "chrome/test/base/chrome_render_view_host_test_harness.h"
#include "components/autofill/core/common/password_form.h"
#include "components/password_manager/core/browser/password_form_manager.h"
#include "components/password_manager/core/browser/password_manager_client.h"
#include "components/password_manager/core/browser/stub_password_manager_client.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {
class MockPasswordFormManager : public password_manager::PasswordFormManager {
 public:
  MOCK_METHOD0(PermanentlyBlacklist, void());

  MockPasswordFormManager(password_manager::StubPasswordManagerClient* client,
                          const autofill::PasswordForm& form)
      : PasswordFormManager(
            nullptr,
            client,
            base::WeakPtr<password_manager::PasswordManagerDriver>(),
            form,
            false) {}

  ~MockPasswordFormManager() override {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MockPasswordFormManager);
};

class TestSavePasswordInfobarDelegate : public SavePasswordInfoBarDelegate {
 public:
  TestSavePasswordInfobarDelegate(
      scoped_ptr<password_manager::PasswordFormManager> form_to_save,
      password_manager::CredentialSourceType source_type)
      : SavePasswordInfoBarDelegate(form_to_save.Pass(),
                                    std::string(),
                                    source_type) {}
  ~TestSavePasswordInfobarDelegate() override {}
};

}  // namespace

class SavePasswordInfoBarDelegateTest : public ChromeRenderViewHostTestHarness {
 public:
  SavePasswordInfoBarDelegateTest();
  ~SavePasswordInfoBarDelegateTest() override{};

  void SetUp() override;
  void TearDown() override;

  const autofill::PasswordForm& test_form() { return test_form_; }
  MockPasswordFormManager* CreateMockFormManager();

 protected:
  scoped_ptr<ConfirmInfoBarDelegate> CreateDelegate(
      MockPasswordFormManager* password_form_manager,
      password_manager::CredentialSourceType type);

  password_manager::StubPasswordManagerClient client_;
  autofill::PasswordForm test_form_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SavePasswordInfoBarDelegateTest);
};

SavePasswordInfoBarDelegateTest::SavePasswordInfoBarDelegateTest() {
  test_form_.origin = GURL("http://example.com");
  test_form_.username_value = base::ASCIIToUTF16("username");
  test_form_.password_value = base::ASCIIToUTF16("12345");
}

MockPasswordFormManager*
SavePasswordInfoBarDelegateTest::CreateMockFormManager() {
  return new MockPasswordFormManager(&client_, test_form());
}

scoped_ptr<ConfirmInfoBarDelegate>
SavePasswordInfoBarDelegateTest::CreateDelegate(
    MockPasswordFormManager* password_form_manager,
    password_manager::CredentialSourceType type) {
  scoped_ptr<ConfirmInfoBarDelegate> delegate(
      new TestSavePasswordInfobarDelegate(
          scoped_ptr<password_manager::PasswordFormManager>(
              password_form_manager),
          type));
  return delegate.Pass();
}

void SavePasswordInfoBarDelegateTest::SetUp() {
  ChromeRenderViewHostTestHarness::SetUp();
}

void SavePasswordInfoBarDelegateTest::TearDown() {
  ChromeRenderViewHostTestHarness::TearDown();
}

TEST_F(SavePasswordInfoBarDelegateTest, CancelTestCredentialSourceAPI) {
  // SavePasswordInfoBarDelegate::Create takes ownership of
  // password_form_manager_ptr;
  MockPasswordFormManager* password_form_manager_ptr = CreateMockFormManager();
  scoped_ptr<ConfirmInfoBarDelegate> infobar(CreateDelegate(
      password_form_manager_ptr,
      password_manager::CredentialSourceType::CREDENTIAL_SOURCE_API));
  EXPECT_CALL(*password_form_manager_ptr, PermanentlyBlacklist())
      .Times(testing::Exactly(0));
  EXPECT_TRUE(infobar->Cancel());
}

TEST_F(SavePasswordInfoBarDelegateTest,
       CancelTestCredentialSourcePasswordManager) {
  // SavePasswordInfoBarDelegate::Create takes ownership of
  // password_form_manager_ptr;
  MockPasswordFormManager* password_form_manager_ptr = CreateMockFormManager();
  scoped_ptr<ConfirmInfoBarDelegate> infobar(CreateDelegate(
      password_form_manager_ptr, password_manager::CredentialSourceType::
                                     CREDENTIAL_SOURCE_PASSWORD_MANAGER));
  EXPECT_CALL(*password_form_manager_ptr, PermanentlyBlacklist())
      .Times(testing::Exactly(1));
  EXPECT_TRUE(infobar->Cancel());
}
