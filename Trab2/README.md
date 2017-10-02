# esquema de relação

```
underline = primary key
italic = not null
strong = foreing key
```

<table>
  <caption>products</caption>
  <tr>
    <th><i>id</i></th>
    <th><u>asin</u></th>
    <th><strong>fk_details<strong/></th>
  </tr>
</table>

<table>
  <caption>details</caption>
  <tr>
    <th>title</th>
    <th>group</th>
    <th>salesrank</th>
    <th>categorie</th>
  </tr>
</table>
