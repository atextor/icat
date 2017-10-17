%global repo_owner atextor
%global repo_name  icat 

Name:    icat
Version: 0.4
Release: 1%{?dist}
Summary: Output images in terminal

License: BSD
URL:     https://github.com/%{repo_owner}/%{repo_name}
Source0: %{URL}/archive/v%{version}.tar.gz#/%{repo_name}-%{version}.tar.gz

BuildRequires: imlib2-devel

%description
Outputs an image on a 256-color or 24-bit color enabled terminal
with UTF-8 locale, such as gnome-terminal, konsole or rxvt-unicode (urxvt).

%prep
%setup -q -n %{repo_name}-%{version}
# Extract license from source code
cat icat.c | awk '1;/\*\//{exit}' > LICENSE

%build
make %{?_smp_mflags}

%install
install -m 755 -d %{buildroot}/%{_bindir}
install -m 755 ./icat %{buildroot}/%{_bindir}/icat

%files
%doc CHANGELOG README.md
%license LICENSE
%{_bindir}/icat

%changelog
* Tue Oct 17 2017 suve <veg@svgames.pl>
- Initial packaging
