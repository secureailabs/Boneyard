# This file helps to compute a version number in source trees obtained from
# git-archive tarball (such as those provided by githubs download-from-tag
# feature). Distribution tarballs (built by setup.py sdist) and build
# directories (produced by setup.py build) will contain a much shorter file
# that just contains the computed version number.

# This file is released into the public domain. Generated by
# versioneer-0.15 (https://github.com/warner/python-versioneer)

import errno
import os
import re
import sys

from pandas.compat import PY3


def get_keywords():
    # these strings will be replaced by git during git-archive.
    # setup.py/versioneer.py will grep for the variable names, so they must
    # each be defined on a line of their own. _version.py will just call
    # get_keywords().
    git_refnames = " (0.24.x)"
    git_full = "95938db1160fef6099421e69a73a48a01f910613"
    keywords = {"refnames": git_refnames, "full": git_full}
    return keywords


class VersioneerConfig(object):
    pass


def get_config():
    # these strings are filled in when 'setup.py versioneer' creates
    # _version.py
    cfg = VersioneerConfig()
    cfg.VCS = "git"
    cfg.style = "pep440"
    cfg.tag_prefix = "v"
    cfg.parentdir_prefix = "pandas-"
    cfg.versionfile_source = "pandas/_version.py"
    cfg.verbose = False
    return cfg


class NotThisMethod(Exception):
    pass


LONG_VERSION_PY = {}
HANDLERS = {}


def register_vcs_handler(vcs, method):  # decorator
    def decorate(f):
        if vcs not in HANDLERS:
            HANDLERS[vcs] = {}
        HANDLERS[vcs][method] = f
        return f
    return decorate

def versions_from_parentdir(parentdir_prefix, root, verbose):
    # Source tarballs conventionally unpack into a directory that includes
    # both the project name and a version string.
    dirname = os.path.basename(root)
    if not dirname.startswith(parentdir_prefix):
        if verbose:
            print("guessing rootdir is '{root}', but '{dirname}' "
                  "doesn't start with prefix '{parentdir_prefix}'".format(
                      root=root, dirname=dirname,
                      parentdir_prefix=parentdir_prefix))
        raise NotThisMethod("rootdir doesn't start with parentdir_prefix")
    return {"version": dirname[len(parentdir_prefix):],
            "full-revisionid": None,
            "dirty": False, "error": None}


@register_vcs_handler("git", "get_keywords")
def git_get_keywords(versionfile_abs):
    # the code embedded in _version.py can just fetch the value of these
    # keywords. When used from setup.py, we don't want to import _version.py,
    # so we do it with a regexp instead. This function is not used from
    # _version.py.
    keywords = {}
    try:
        f = open(versionfile_abs, "r")
        for line in f.readlines():
            if line.strip().startswith("git_refnames ="):
                mo = re.search(r'=\s*"(.*)"', line)
                if mo:
                    keywords["refnames"] = mo.group(1)
            if line.strip().startswith("git_full ="):
                mo = re.search(r'=\s*"(.*)"', line)
                if mo:
                    keywords["full"] = mo.group(1)
        f.close()
    except EnvironmentError:
        pass
    return keywords


@register_vcs_handler("git", "keywords")
def git_versions_from_keywords(keywords, tag_prefix, verbose):
    if not keywords:
        raise NotThisMethod("no keywords at all, weird")
    refnames = keywords["refnames"].strip()
    if refnames.startswith("$Format"):
        if verbose:
            print("keywords are unexpanded, not using")
        raise NotThisMethod("unexpanded keywords, not a git-archive tarball")
    refs = {r.strip() for r in refnames.strip("()").split(",")}
    # starting in git-1.8.3, tags are listed as "tag: foo-1.0" instead of
    # just "foo-1.0". If we see a "tag: " prefix, prefer those.
    TAG = "tag: "
    tags = {r[len(TAG):] for r in refs if r.startswith(TAG)}
    if not tags:
        # Either we're using git < 1.8.3, or there really are no tags. We use
        # a heuristic: assume all version tags have a digit. The old git %d
        # expansion behaves like git log --decorate=short and strips out the
        # refs/heads/ and refs/tags/ prefixes that would let us distinguish
        # between branches and tags. By ignoring refnames without digits, we
        # filter out many common branch names like "release" and
        # "stabilization", as well as "HEAD" and "master".
        tags = {r for r in refs if re.search(r'\d', r)}
        if verbose:
            print("discarding '{}', no digits".format(",".join(refs - tags)))
    if verbose:
        print("likely tags: {}".format(",".join(sorted(tags))))
    for ref in sorted(tags):
        # sorting will prefer e.g. "2.0" over "2.0rc1"
        if ref.startswith(tag_prefix):
            r = ref[len(tag_prefix):]
            if verbose:
                print("picking {r}".format(r=r))
            return {"version": r,
                    "full-revisionid": keywords["full"].strip(),
                    "dirty": False, "error": None
                    }
    # no suitable tags, so version is "0+unknown", but full hex is still there
    if verbose:
        print("no suitable tags, using unknown + full revision id")
    return {"version": "0+unknown",
            "full-revisionid": keywords["full"].strip(),
            "dirty": False, "error": "no suitable tags"}


def plus_or_dot(pieces):
    if "+" in pieces.get("closest-tag", ""):
        return "."
    return "+"


def render_pep440(pieces):
    # now build up version string, with post-release "local version
    # identifier". Our goal: TAG[+DISTANCE.gHEX[.dirty]] . Note that if you
    # get a tagged build and then dirty it, you'll get TAG+0.gHEX.dirty

    # exceptions:
    # 1: no tags. git_describe was just HEX. 0+untagged.DISTANCE.gHEX[.dirty]

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        if pieces["distance"] or pieces["dirty"]:
            rendered += plus_or_dot(pieces)
            rendered += "{:d}.g{}".format(pieces["distance"], pieces["short"])
            if pieces["dirty"]:
                rendered += ".dirty"
    else:
        # exception #1
        rendered = "0+untagged.{:d}.g{}".format(pieces["distance"],
                                                pieces["short"])
        if pieces["dirty"]:
            rendered += ".dirty"
    return rendered


def render_pep440_pre(pieces):
    # TAG[.post.devDISTANCE] . No -dirty

    # exceptions:
    # 1: no tags. 0.post.devDISTANCE

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        if pieces["distance"]:
            rendered += ".post.dev%d" % pieces["distance"]
    else:
        # exception #1
        rendered = "0.post.dev%d" % pieces["distance"]
    return rendered


def render_pep440_post(pieces):
    # TAG[.postDISTANCE[.dev0]+gHEX] . The ".dev0" means dirty. Note that
    # .dev0 sorts backwards (a dirty tree will appear "older" than the
    # corresponding clean one), but you shouldn't be releasing software with
    # -dirty anyways.

    # exceptions:
    # 1: no tags. 0.postDISTANCE[.dev0]

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        if pieces["distance"] or pieces["dirty"]:
            rendered += ".post{:d}".format(pieces["distance"])
            if pieces["dirty"]:
                rendered += ".dev0"
            rendered += plus_or_dot(pieces)
            rendered += "g{}".format(pieces["short"])
    else:
        # exception #1
        rendered = "0.post%d" % pieces["distance"]
        if pieces["dirty"]:
            rendered += ".dev0"
        rendered += "+g{}".format(pieces["short"])
    return rendered


def render_pep440_old(pieces):
    # TAG[.postDISTANCE[.dev0]] . The ".dev0" means dirty.

    # exceptions:
    # 1: no tags. 0.postDISTANCE[.dev0]

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        if pieces["distance"] or pieces["dirty"]:
            rendered += ".post%d" % pieces["distance"]
            if pieces["dirty"]:
                rendered += ".dev0"
    else:
        # exception #1
        rendered = "0.post%d" % pieces["distance"]
        if pieces["dirty"]:
            rendered += ".dev0"
    return rendered


def render_git_describe(pieces):
    # TAG[-DISTANCE-gHEX][-dirty], like 'git describe --tags --dirty
    # --always'

    # exceptions:
    # 1: no tags. HEX[-dirty]  (note: no 'g' prefix)

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        if pieces["distance"]:
            rendered += "-{:d}-g{}".format(pieces["distance"], pieces["short"])
    else:
        # exception #1
        rendered = pieces["short"]
    if pieces["dirty"]:
        rendered += "-dirty"
    return rendered


def render_git_describe_long(pieces):
    # TAG-DISTANCE-gHEX[-dirty], like 'git describe --tags --dirty
    # --always -long'. The distance/hash is unconditional.

    # exceptions:
    # 1: no tags. HEX[-dirty]  (note: no 'g' prefix)

    if pieces["closest-tag"]:
        rendered = pieces["closest-tag"]
        rendered += "-{:d}-g{}".format(pieces["distance"], pieces["short"])
    else:
        # exception #1
        rendered = pieces["short"]
    if pieces["dirty"]:
        rendered += "-dirty"
    return rendered


def render(pieces, style):
    if pieces["error"]:
        return {"version": "unknown",
                "full-revisionid": pieces.get("long"),
                "dirty": None,
                "error": pieces["error"]}

    if not style or style == "default":
        style = "pep440"  # the default

    if style == "pep440":
        rendered = render_pep440(pieces)
    elif style == "pep440-pre":
        rendered = render_pep440_pre(pieces)
    elif style == "pep440-post":
        rendered = render_pep440_post(pieces)
    elif style == "pep440-old":
        rendered = render_pep440_old(pieces)
    elif style == "git-describe":
        rendered = render_git_describe(pieces)
    elif style == "git-describe-long":
        rendered = render_git_describe_long(pieces)
    else:
        raise ValueError("unknown style '{style}'".format(style=style))

    return {"version": rendered, "full-revisionid": pieces["long"],
            "dirty": pieces["dirty"], "error": None}


def get_versions():
    # I am in _version.py, which lives at ROOT/VERSIONFILE_SOURCE. If we have
    # __file__, we can work backwards from there to the root. Some
    # py2exe/bbfreeze/non-CPython implementations don't do __file__, in which
    # case we can only use expanded keywords.

    cfg = get_config()
    verbose = cfg.verbose

    try:
        return git_versions_from_keywords(get_keywords(), cfg.tag_prefix,
                                          verbose)
    except NotThisMethod:
        pass

    try:
        root = os.path.realpath(__file__)
        # versionfile_source is the relative path from the top of the source
        # tree (where the .git directory might live) to this file. Invert
        # this to find the root from __file__.
        for i in cfg.versionfile_source.split('/'):
            root = os.path.dirname(root)
    except NameError:
        return {"version": "0+unknown", "full-revisionid": None,
                "dirty": None,
                "error": "unable to find root of source tree"}

    try:
        if cfg.parentdir_prefix:
            return versions_from_parentdir(cfg.parentdir_prefix, root, verbose)
    except NotThisMethod:
        pass

    return {"version": "0+unknown", "full-revisionid": None,
            "dirty": None,
            "error": "unable to compute version"}
