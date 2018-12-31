module.exports = {
  env: [
    {
      label: 'comment',
      mode: 'text',
      name: $ => $.env_name_comment,
      bare: true,
      // The comment environment provided does not really have any parameters
      // but the eol makes it possible to treat all possible comment
      // environments as a single rule.
      beginParameters: $ => [
        repeat(
          choice(
            $.text,
            $.brack_group,
            $.group
          )
        ),
        $.eol
      ],
      endParameters: $ => [
        optional(alias($.ignored_line, $.ignored))
      ],
      contents: $ => [alias($.verbatim, $.comment_block)]
    },
    {
      label: 'verbatim',
      mode: 'text',
      name: $ => $.env_name_verbatim,
      bare: true,
      // The verbatim environment provided does not really have any parameters
      // but the eol makes it possible to treat all possible verbatim
      // environments as a single rule.
      beginParameters: $ => [
        repeat(
          choice(
            $.text,
            $.brack_group,
            $.group
          )
        ),
        $.eol
      ],
      endParameters: $ => [
        optional(alias($.ignored_line, $.ignored))
      ],
      contents: $ => [$.verbatim]
    }
  ]
}
