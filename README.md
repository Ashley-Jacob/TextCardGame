# Card Game Overview

I created a text-based version of a card game heavily inspired by Uno and Crazy Eights.

Note: Game is not entirely finished, so there may be future adjustments to the code.

## Rules

1. Enter a random number at the start of the game to set the seed
2. Game can be played with 1-4 CPUs
3. Cards can be played if they have the same color or value as the active card
    1. Enter the corresponding number next to a card in your hand to play it
    2. You may enter 0 to draw a card
        1. You may draw a card even if you could've played something
4. There are certain cards with special abilities
    1. Wild: Player can pick a color to switch to
    2. Gift: Player selects a card from their hand to gift to the next player
       1. No card is gifted if the player has no more cards in their hand
    3. Skip: Next player's turn is skipped
    4. Vortex: Player can pick a color to switch to. Every player gifts a card to the next player.
    5. Draw 3: Next player draws 3 cards (their turn is not skipped)
5. Game ends when the human player runs out of cards or is the last player left
    1. If the human player finishes early, remaining CPUs are ranked based on the numbers of cards they have left
