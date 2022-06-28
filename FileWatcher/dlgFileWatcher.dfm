object FormFileWatcher: TFormFileWatcher
  Left = 0
  Top = 0
  Caption = 'File Watcher'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  DesignSize = (
    624
    441)
  TextHeight = 15
  object edtPfad: TEdit
    Left = 8
    Top = 8
    Width = 608
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object btnStart: TButton
    Left = 8
    Top = 37
    Width = 89
    Height = 25
    Caption = 'Ohne Thread'
    TabOrder = 1
    OnClick = btnOhneThreadClick
  end
  object meInfo: TMemo
    Left = 8
    Top = 68
    Width = 608
    Height = 365
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
  end
  object btnMitThread: TButton
    Left = 103
    Top = 37
    Width = 89
    Height = 25
    Caption = 'Mit Thread'
    TabOrder = 3
    OnClick = btnMitThreadClick
  end
end
