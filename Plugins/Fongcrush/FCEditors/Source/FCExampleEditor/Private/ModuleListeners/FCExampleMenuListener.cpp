
#include "ModuleListeners/FCExampleMenuListener.h"
#include "FCExampleEditor.h"
#include "Selection.h"

#define LOCTEXT_NAMESPACE "FFCMenuListener"

class FMenuToolCommands : public TCommands<FMenuToolCommands>
{
public:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	FUNCTIONS
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	FMenuToolCommands() : TCommands<FMenuToolCommands>(
		TEXT("FFCExampleMenuListener"),
		FText::FromString("FCExample Menu tool"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(
			MenuCommand01,
			"Menu Command 01",
			"Test Menu Command 01.",
			EUserInterfaceActionType::Button,
			FInputGesture());

		UI_COMMAND(
			SubCommand01,
			"서브 메뉴 명령 1",
			"Test Menu Command 02.",
			EUserInterfaceActionType::Button,
			FInputGesture());

		UI_COMMAND(
			SubCommand02,
			"서브 메뉴 명령 2",
			"Test Menu Command 03.",
			EUserInterfaceActionType::Button,
			FInputGesture());
	}

	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓	
	TSharedPtr<FUICommandInfo> MenuCommand01;
	TSharedPtr<FUICommandInfo> SubCommand01;
	TSharedPtr<FUICommandInfo> SubCommand02;
};

void FFCExampleMenuListener::OnStartupModule()
{
	UICommandList = MakeShared<FUICommandList>();

	// 명령 등록
	FMenuToolCommands::Register();
	// 명령 바인드
	BindCommands();
	// 메뉴 확장 등록
	FFCExampleEditor::Get().AddMenuExtension(
		FName("Section_1"),
		FMenuExtensionDelegate::CreateRaw(this, &FFCExampleMenuListener::MakeMenuEntry),
		EExtensionHook::After,
		UICommandList);
}

void FFCExampleMenuListener::OnShutdownModule()
{
	FMenuToolCommands::Unregister();
}

void FFCExampleMenuListener::MakeMenuEntry(FMenuBuilder& MenuBuilder)
{
	const FMenuToolCommands& Commands = FMenuToolCommands::Get();
	MenuBuilder.AddMenuEntry(Commands.MenuCommand01);
	MenuBuilder.AddSubMenu(
		FText::FromString("Sub Menu"),
		FText::FromString("서브 메뉴입니다"),
		FNewMenuDelegate::CreateSP(this, &FFCExampleMenuListener::MakeSubMenu));

	TextWidget = SNew(SEditableText)
		.MinDesiredWidth(50)
		.IsReadOnly(true)
		.Text(FText::FromString(GetSelectedActorNames()));
	
	TSharedRef<SWidget> SelectedNamesWidget =
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			TextWidget.ToSharedRef()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5, 0, 0, 0)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.Text(FText::FromString("Get Actors Name"))
			.OnClicked(FOnClicked::CreateLambda([this]()->FReply
			{
				TextWidget->SetText(FText::FromString(GetSelectedActorNames()));
				return FReply::Handled();
			}))
		];
	MenuBuilder.AddWidget(SelectedNamesWidget, FText::FromString(TEXT("엑터명")));
}

void FFCExampleMenuListener::MakeSubMenu(FMenuBuilder& MenuBuilder)
{
	const FMenuToolCommands& Commands = FMenuToolCommands::Get();
	MenuBuilder.AddMenuEntry(Commands.SubCommand01);
	MenuBuilder.AddMenuEntry(Commands.SubCommand02);
}

void FFCExampleMenuListener::BindCommands()
{
	const FMenuToolCommands& Commands = FMenuToolCommands::Get();
	UICommandList->MapAction(
		Commands.MenuCommand01,
		FExecuteAction::CreateSP(this, &FFCExampleMenuListener::MenuCommand01),
		FCanExecuteAction());

	UICommandList->MapAction(
		Commands.SubCommand01,
		FExecuteAction::CreateSP(this, &FFCExampleMenuListener::MenuCommand01),
		FCanExecuteAction());

	UICommandList->MapAction(
		Commands.SubCommand02,
		FExecuteAction::CreateSP(this, &FFCExampleMenuListener::MenuCommand01),
		FCanExecuteAction());
}

void FFCExampleMenuListener::MenuCommand01()
{
	UE_LOG(LogTemp, Warning, TEXT("Menu Tool 명령 실행"));
}

FString FFCExampleMenuListener::GetSelectedActorNames()
{
	const FScopedTransaction Transection(FText::FromString("Get Actor Name"));

	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
	FString SelectedActorNames;
	for (const AActor* const& SelectedActor : SelectedActors)
	{
		SelectedActorNames.Append(SelectedActor->GetName());
	}
	
	return SelectedActorNames;
}

#undef LOCTEXT_NAMESPACE
