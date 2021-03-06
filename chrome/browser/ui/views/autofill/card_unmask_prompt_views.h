// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_AUTOFILL_CARD_UNMASK_PROMPT_VIEWS_H_
#define CHROME_BROWSER_UI_VIEWS_AUTOFILL_CARD_UNMASK_PROMPT_VIEWS_H_

#include "chrome/browser/ui/autofill/autofill_dialog_models.h"
#include "chrome/browser/ui/autofill/card_unmask_prompt_view.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/gfx/animation/slide_animation.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {
class ImageView;
class Label;
class Checkbox;
class CheckmarkThrobber;
}

namespace autofill {

class DecoratedTextfield;

class CardUnmaskPromptViews : public CardUnmaskPromptView,
                              views::ComboboxListener,
                              views::DialogDelegateView,
                              views::TextfieldController,
                              gfx::AnimationDelegate {
 public:
  explicit CardUnmaskPromptViews(CardUnmaskPromptController* controller);

  ~CardUnmaskPromptViews() override;

  void Show();

  // CardUnmaskPromptView
  void ControllerGone() override;
  void DisableAndWaitForVerification() override;
  void GotVerificationResult(const base::string16& error_message,
                             bool allow_retry) override;

  // views::DialogDelegateView
  View* GetContentsView() override;
  views::View* CreateFootnoteView() override;

  // views::View
  gfx::Size GetPreferredSize() const override;
  void Layout() override;
  int GetHeightForWidth(int width) const override;
  void OnNativeThemeChanged(const ui::NativeTheme* theme) override;
  ui::ModalType GetModalType() const override;
  base::string16 GetWindowTitle() const override;
  void DeleteDelegate() override;
  int GetDialogButtons() const override;
  base::string16 GetDialogButtonLabel(ui::DialogButton button) const override;
  bool ShouldDefaultButtonBeBlue() const override;
  bool IsDialogButtonEnabled(ui::DialogButton button) const override;
  views::View* GetInitiallyFocusedView() override;
  bool Cancel() override;
  bool Accept() override;

  // views::TextfieldController
  void ContentsChanged(views::Textfield* sender,
                       const base::string16& new_contents) override;
  // views::ComboboxListener
  void OnPerformAction(views::Combobox* combobox) override;

  // gfx::AnimationDelegate
  void AnimationProgressed(const gfx::Animation* animation) override;

 private:
  friend class CardUnmaskPromptViewTesterViews;

  // A view that allows changing the opacity of its contents.
  class FadeOutView : public views::View {
   public:
    FadeOutView();
    ~FadeOutView() override;

    // views::View
    void PaintChildren(const ui::PaintContext& context) override;
    void OnPaint(gfx::Canvas* canvas) override;

    void set_fade_everything(bool fade_everything) {
      fade_everything_ = fade_everything;
    }
    void SetOpacity(double opacity);

   private:
    // Controls whether the background and border are faded out as well. Default
    // is false, meaning only children are faded.
    bool fade_everything_;

    // On a scale of 0-1, how much to fade out the contents of this view. 0 is
    // totally invisible, 1 is totally visible.
    double opacity_;

    DISALLOW_COPY_AND_ASSIGN(FadeOutView);
  };

  void InitIfNecessary();
  void SetRetriableErrorMessage(const base::string16& message);
  bool ExpirationDateIsValid() const;
  void SetInputsEnabled(bool enabled);
  void ClosePrompt();

  CardUnmaskPromptController* controller_;

  views::View* main_contents_;

  // The error label for permanent errors (where the user can't retry).
  views::Label* permanent_error_label_;

  // Holds the cvc and expiration inputs.
  views::View* input_row_;

  DecoratedTextfield* cvc_input_;

  // These will be null when expiration date is not required.
  views::Combobox* month_input_;
  views::Combobox* year_input_;

  MonthComboboxModel month_combobox_model_;
  YearComboboxModel year_combobox_model_;

  // The error icon and label for most errors, which live beneath the inputs.
  views::ImageView* error_icon_;
  views::Label* error_label_;

  FadeOutView* storage_row_;
  views::Checkbox* storage_checkbox_;

  FadeOutView* progress_overlay_;
  views::CheckmarkThrobber* progress_throbber_;
  views::Label* progress_label_;

  gfx::SlideAnimation overlay_animation_;

  base::WeakPtrFactory<CardUnmaskPromptViews> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(CardUnmaskPromptViews);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_VIEWS_AUTOFILL_CARD_UNMASK_PROMPT_VIEWS_H_
