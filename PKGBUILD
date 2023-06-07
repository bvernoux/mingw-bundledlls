# Maintainer: Benjamin Vernoux <bvernoux@gmail.com>
pkgname=mingw-bundledlls
pkgver=$(curl -s "https://api.github.com/repos/bvernoux/mingw-bundledlls/tags" | grep '"name":' | sed -E 's/.*"([^"]+)".*/\1/' | head -n1 | sed 's/^v//')
pkgrel=1
pkgdesc="C native app that takes an EXE file, automatically figures out all the DLL dependencies, and copies them next to the EXE."
arch=('x86_64')
url="https://github.com/bvernoux/mingw-bundledlls"
license=('MIT')

# Specify the dependencies for building the package
makedepends=('mingw-w64-x86_64-gcc')

# Fetching the source code from the repository
source=("https://github.com/bvernoux/mingw-bundledlls/archive/v${pkgver}.tar.gz")
sha256sums=('SKIP')

# Build the package
build() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  make
}

# Install the package
package() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  install -Dm755 ${pkgname}.exe "${pkgdir}/usr/bin/${pkgname}.exe"
}

# Specify the package metadata
pkgver() {
  echo "$pkgver"
}

# Uncomment the following line if you want to sign the package
# validpgpkeys=('KEY_ID')

# Uncomment the following lines if you want to install the package in /mingw64 instead of /usr
# package() {
#   cd "${srcdir}/${pkgname}-${pkgver}"
#   install -Dm755 ${pkgname}.exe "${pkgdir}/mingw64/usr/bin/${pkgname}.exe"
# }
