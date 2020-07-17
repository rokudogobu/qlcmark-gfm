cask 'qlcmark-gfm' do
  version '1.3.1'
  sha256 '6a786ce0199220a0f09942fb9388c8676ba28cd9e6be8e619812fb4377d83c53'

  url "https://github.com/rokudogobu/qlcmark-gfm/releases/download/v#{version}/qlcmark-gfm.qlgenerator.zip"
  appcast 'https://github.com/rokudogobu/qlcmark-gfm/releases.atom'
  name 'qlcmark-gfm'
  homepage 'https://github.com/rokudogobu/qlcmark-gfm'

  qlplugin 'qlcmark-gfm.qlgenerator'
end
